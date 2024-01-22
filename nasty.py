#! /usr/bin/env python

# nasty.py: Generate random test cases
#
# How to use:
#
#	 ./nasty.py --nodes 2 --tasks 30 > nasty.c
#	 mcc -o nasty --ompss-2 nasty.c
#	 mpirun -np 2 ./nasty
#
# If it passes, then it will finish either with
#
#	 Test passed [./nasty.py --nodes 2 --tasks 30]
#
# If the data has incorrect values, then it will give an assertion failure, e.g.:
#
#	 Error in check_20
#	 nasty: nasty.c:47: void check_hash(const int *, int, int, const char *): Assertion `val == hash' failed.
#
# Then repeat with the --no-hash option
#
#	 ./nasty.py --no-hash --nodes 2 --tasks 30 > nasty.c
#
# The output will indicate which data value is incorrect:
#
#	 check_20 a[500]: 0x00000000 != 0x656b9b23
#	 nasty: nasty.c:27: void check(const int *, const int *, int, const char *, int): Assertion `a[j] == ref[j]' failed.
#
# In this case add the --track a[500] option, which keeps only the accesses to array element a[500], on which
# there was a failure:
# 
#	 ./nasty.py --no-hash --track a[500] --nodes 2 --tasks 30
#
# The error message will be the same as before, but the test program will be simpler.

import sys
import random
import getopt
import copy
import re
import bisect
import math
from string import Template

# Default parameters
max_nesting_level = 3	 # --nesting=
max_num_tasks = 1000	 # --tasks=
use_hashing = True		 # --no-hash
use_dmalloc = True		 # Set to false by --serial
no_taskwait = False		 # Set to true by --no-taskwait
nodes = None			 # Number of nodes for node() clause if not None
track = None			 # --track
branching = None		 # Average number of subtasks per task
taskwaiton = True       # Generate #pragma taskwait on
if0 = True

# Use separate random number generators for program structure and for data
# contents (so can the presence/absence of --no-hash or --track does not change
# the program structure)

random_structure = random.Random()
random_contents = random.Random()
random_nodes = random.Random()

def Usage():
	print 'nasty.py'
	print '-h, --help		 Show this help'
	print '--seed string	 Set random seed'
	print '--tasks t		 Maximum number of tasks to generate (excluding main)'
	print '--nesting l		 Maximum nesting level (excluding main)'
	print '--nodes n		 Use node() clause to randomly assign tasks to nodes, assuming n nodes'
	print '--no-taskwait	 Do not use taskwaits'
	print '--no-taskwaiton	 Do not use taskwaiton'
	print '--serial			 Do not use nanos6.h and substitute malloc for dmalloc/lmalloc'
	print '--no-hash		 Generate and check arrays using random test data (rather than pseudorandom generator and hashes)'
	print '--track desc		 Track only certain locations; e.g. --track b[789], useful for debugging'
	print '--branching b	 Average number of subtasks per task'
	return 1


# The beginning of the program
all_prefix = """// $arg_summary

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
$nanos6_include

void show(const int *a, int n)
{
	printf(\"Show %p size %d\\n\", a, n);
	for (int j=0; j<n; j++)
	{
		printf(\"0x%08x\\n\", a[j]);
	}
}

void check(const int *a, const int *ref, int n, const char *name, int ofs)
{
	// printf(\"Check %p size %d\\n\", a, n);
	for (int j=0; j<n; j++)
	{
		if (a[j] != ref[j])
		{
			const char *eq = (a[j] == ref[j]) ? \"==\" : \"!=\";
			printf(\"%s[%d]: 0x%08x %s 0x%08x\\n\", name, ofs+j, a[j], eq, ref[j]);
			assert_that(a[j] == ref[j]);
		}
	}
}

void copy(int *a, const int *ref, int n)
{
	memcpy(a, ref, n * sizeof(int));
}

void check_hash(const int *a, int hash, int n, const char *name)
{
	int val = 0;
	for (int j=0; j<n; j++)
	{
		val ^= a[j];
		val = ((val * 1103515245) + 12345) & 0x7fffffff;
	}
	if (val != hash)
		printf("Error in %s\\n", name);
	assert_that(val == hash);
}

void fill(int *a, int val, int n)
{
	for (int j=0; j<n; j++)
	{
		a[j] = val;
		val = ((val * 1103515245) + 12345) & 0x7fffffff;
	}
}

"""

# Level of access currently given to each element of an array
# 'Read': in dependency
# 'Write': out dependency not written to yet
# 'ReadWrite': inout dependency (note: NOT out dependency after writing)
# 'Read_Subtasks': as for 'Read', but only by subtasks (e.g. weakin dependency)
# ...
access_levels = [ 'NoAccess', 'Read', 'Write', 'ReadWrite', 'Read_Subtasks', 'Write_Subtasks', 'ReadWrite_Subtasks']

# Task access types (dependency clauses)
subtask_access_types = [ 'In', 'Inout', 'Out', 'Weakin', 'Weakinout', 'Weakout']
direct_access_types = ['Directin', 'Directout']  # describes when "child access" is actually read or write in the task itself
all_access_types = subtask_access_types + direct_access_types

# What parent task access levels are acceptable for each child access type
type_to_ok_levels = {  'In'    : ['Read', 'ReadWrite', 'Read_Subtasks', 'ReadWrite_Subtasks'],
					   'Inout' : ['ReadWrite', 'ReadWrite_Subtasks'],
					   'Out'   : ['Write', 'ReadWrite', 'Write_Subtasks', 'ReadWrite_Subtasks'],
					   'Weakin'    : ['Read', 'ReadWrite', 'Read_Subtasks', 'ReadWrite_Subtasks'],
					   'Weakinout' : ['ReadWrite', 'ReadWrite_Subtasks'],
					   'Weakout'   : ['Write', 'ReadWrite', 'Write_Subtasks', 'ReadWrite_Subtasks'],
					   'Directin'  : ['Read', 'ReadWrite'],
					   'Directinout' : ['ReadWrite'],
					   'Directout'	 : ['Write', 'ReadWrite'],
					   'NoDependency' : access_levels }

def level_only_subtasks(level):
	r = {'Read' : 'Read_Subtasks', 
		 'Write' : 'Write_Subtasks',
		 'ReadWrite' : 'ReadWrite_Subtasks'}
	return r.get(level, level)

def level_make_strong(level):
	r = {'Read_Subtasks' : 'Read', 
		 'Write_Subtasks' : 'Write',
		 'ReadWrite_Subtasks' : 'ReadWrite'}
	return r.get(level, level)

def accessible_by_child(have, childtype):
	return have in type_to_ok_levels[childtype]

def is_weak(access_type):
	return access_type in ('Weakin', 'Weakinout', 'Weakout')

# An array, it's current level of access in the current and parent tasks and
# the most up-to-date reference data.
class Array:
	def __init__(self, name, array_len):
		self.array_len = array_len
		self.name = name
		self.ref = [0] * array_len
		self.level = ['Write'] * array_len
		self.old_levels = []
		self.already_checked = [False] * array_len
		self.already_written = [False] * array_len

	# Enter subtask, so push current access levels
	def push_level(self):
		self.old_levels.append(copy.copy(self.level))

	# Leave subtask, so discard parent's old access levels
	def pop_level(self):
		assert len(self.old_levels) > 0
		self.old_levels = self.old_levels[:-1]

	# Set child's access level for a region
	def child_level(self, ofs, size, childtype):
		for j in range(ofs, ofs+size):
			have = self.level[j]
			assert accessible_by_child(have, childtype)

			# Ignore whether parent can access the data
			have = level_only_subtasks(have)

			if childtype in ('In', 'Weakin'):
				have = 'Read_Subtasks' # Child will only be able to read
			elif childtype in ('Out', 'Weakout'):
				have = 'Write_Subtasks' # Child will initially only be able to write

			# Give direct access if child has a strong dependency
			if not is_weak(childtype):
				have = level_make_strong(have)

			self.level[j] = have

	# Set parent's access level for a region after the child completes
	def set_level_after_subtask(self, ofs, size, childtype):
		for j in range(ofs, ofs+size):
			have = self.old_levels[-1][j] # parent's old status before child
			childleaves = self.level[j]   # child's final status

			oldhave = have
			# Only accessible now by subtasks
			have = level_only_subtasks(have)

			if childleaves in ('Write', 'Write_Subtasks'):
				# At some point an out dependency wasn't fully written to
				# so it is now write-only (by subtasks)
				assert have in ('Write_Subtasks', 'ReadWrite_Subtasks')
				have = 'Write_Subtasks'

			self.level[j] = have

	# Set parent's access level for a region after a taskwait
	def set_level_after_taskwait(self, ofs, size, accesstype):
		if is_weak(accesstype):
			#Do nothing for weak dependencies
			return
		for j in range(ofs, ofs+size):
			have = self.level[j] # parent's old status before child
			oldhave = have

			# Now accessible by the task
			have = level_make_strong(have)

			self.level[j] = have

	# Set parent's access level for a region after a taskwaiton
	def set_level_after_taskwaiton(self, ofs, size, accesstype):
		# Unlike a normal taskwait, taskwaiton will bring weak dependencies
		for j in range(ofs, ofs+size):
			have = self.level[j] # parent's old status before child
			oldhave = have

			# Now accessible by the task
			have = level_make_strong(have)

			self.level[j] = have

	# Set own access level after a local ("direct") read or write
	def set_level_after_direct(self, ofs, size, accesstype):
		for j in range(ofs, ofs+size):
			have = self.level[j]
			if accesstype == 'Directin':
				assert have in ('Read', 'ReadWrite')
				have = 'ReadWrite'
			elif accesstype == 'Directout':
				assert have in ('Write', 'ReadWrite')
				have = 'ReadWrite'
			else:
				assert False
			self.level[j] = have

	# Notify whether the contents of a region have already been checked
	# so no need to check again
	def set_checked(self, ofs, size, checked):
		for j in range(0, size):
			self.already_checked[j+ofs] = checked

	# Notify that contents of a region have been written to so it would
	# be pointless to write to it again
	def set_written(self, ofs, size, written):
		assert len(self.already_written) == self.array_len
		assert size + ofs <= self.array_len
		for j in range(0, size):
			self.already_written[j+ofs] = written

	def update_ref(self, ofs, new_contents):
		size = len(new_contents)
		for j in range(0, size):
			self.ref[j+ofs] = new_contents[j]
			#self.already_checked[j+ofs] = False
			self.already_written[j+ofs] = True

	def get_ref(self, ofs, size):
		return self.ref[ofs:ofs+size]

	def child_access_valid(self, ofs, size, accesstype):
		oklevels = type_to_ok_levels[accesstype]
		for j in range(ofs, ofs+size):
			if not self.level[j] in oklevels:
				return False
		return True
		
# Array access with region and dependency type
class ArrayAccess:
	def __init__(self, accesstype, array, ofs, size):
		self.accesstype = accesstype
		self.array = array
		self.ofs = ofs
		self.size = size
		self.new_contents = None

		if accesstype in ('Out', 'Inout', 'Directout', 'Directinout'):
			self.new_contents = [random_contents.randint(0,1000) for j in range(0,size) ]

	def accessible_ofs(self, ofs, wanted):
		return accessible_by_child(self.array.level[ofs], wanted)

	def random_subaccess(self, wanted):

		# Starting point
		start_ofs = random_structure.randint(self.ofs, self.ofs + self.size-1)
		
		ofs = start_ofs
		while not self.accessible_ofs(ofs, wanted):
			ofs = ofs + 1
			if ofs == self.ofs + self.size:
				ofs = self.ofs
			if ofs == start_ofs:
				return None, None

		target_size = random_structure.randint(0, self.ofs + self.size - ofs)
		size = 1
		while size < target_size and self.accessible_ofs(ofs + size, wanted):
			size += 1

		return ofs, size

	def accessible_parts(self, wanted, ofs=None, size=None):
		if ofs is None:
			ofs = self.ofs
			size = self.size
		assert ofs >= self.ofs
		end = ofs + size
		assert end <= self.ofs + self.size

		while ofs < end:
			# Find a accessible part
			while not self.accessible_ofs(ofs, wanted):
				ofs += 1
				if ofs >= end:
					return
			size = 1
			while ofs+size < end and self.accessible_ofs(ofs+size, wanted):
				size = size + 1
			yield ofs, size
			ofs += size
		
	def unchecked_parts(self, ofs=None, size=None):
		if ofs is None:
			ofs = self.ofs
			size = self.size
		assert ofs >= self.ofs
		end = ofs + size
		assert end <= self.ofs + self.size

		while ofs < end:
			# Find an unchecked part
			while self.array.already_checked[ofs]:
				ofs += 1
				if ofs >= end:
					return
			size = 1
			while ofs+size < end and not self.array.already_checked[ofs+size]:
				size = size + 1
			yield ofs, size
			ofs += size
		
	def unwritten_parts(self, ofs=None, size=None):
		if ofs is None:
			ofs = self.ofs
			size = self.size
		assert ofs >= self.ofs
		end = ofs + size
		assert end <= self.ofs + self.size

		while ofs < end:
			# Find an unchecked part
			while self.array.already_written[ofs]:
				ofs += 1
				if ofs >= end:
					return
			size = 1
			while ofs+size < end and not self.array.already_written[ofs+size]:
				size = size + 1
			yield ofs, size
			ofs += size
		
	def tracked_parts(self, ofs, size):
		global track
		if track is None:
			yield (ofs, size)
		else:
			for (aname, index) in track:
				if aname == self.array.name:
					for i in range(ofs, ofs+size):
						if i == index:
							yield (index, 1)
				

# A random operation; e.g. subtask, taskwait or direct write
class Operation:
	def __init__(self, optype):
		self.optype = optype

class Subtask(Operation):
	def __init__(self, arrayaccess):
		self.arrayaccess = arrayaccess
		self.subopts = []
		Operation.__init__('Subtask')

class Taskwait(Operation):
	def __init__(self):
		Operation.__init__('Taskwait')

class DirectWrite(Operation):
	def __init__(self):
		Operation.__init__('DirectWrite')

array_number = 0
array_memo = {}

# Arrays need to come at the beginning of the program since otherwise
# memoization may mean that a reference to an array that was generated earlier
# may actually be serialized after the definition of the array
all_arrays = []


def generate_array(prefix, contents):
	global array_number
	global array_memo
	key = tuple(contents)
	if key in array_memo.keys():
		return array_memo[key]
	array_name = '%s_%d' % (prefix,array_number)
	array_memo[key] = array_name
	array_number += 1
	all_arrays.append('int %s[%d] = {' % (array_name, len(contents)))
	ofs = 0 
	l = len(contents)
	while ofs < l:
		line = ' ' * 10
		for j in range(0,4):
			if ofs < l:
				line += '0x%08x%s ' % (contents[ofs], ',' if ofs < (l-1) else '')
				ofs += 1
			else:
				break
		all_arrays.append(line)
	all_arrays.append('};')
	return array_name

def generate_random_array(size):
	contents = [ random_contents.randint(0,0xffffffff) for j in range(0, size) ]
	array_name =  generate_array('out', contents)
	return array_name, contents

def next_val(val):
	return ((val * 1103515245) + 12345) & 0x7fffffff

def generate_hash_array(size):
	new_contents = []
	inithash = random_contents.randint(0, 0xffffffff)
	val = inithash
	for j in range(0, size):
		new_contents.append(val)
		val = next_val(val)

	return new_contents, inithash

def calc_hash(contents):
	val = 0
	for c in contents:
		val ^= c
		val = next_val(val)
	return val


def make_pragma_clause(array, ofs, size, accesstype):
	return '%s(%s[%d;%d])' % (accesstype.lower(), array.name, ofs, size)


subfunc_number = 0

def generate_subfunc(all_statements, args, accesses, pragma, nesting_level, node):
	global subfunc_number
	subfunc_name = 'func%d' % subfunc_number
	subfunc_number += 1

	prefix = []
	if not pragma is None:
		prefix.append(pragma)
	prefix.append('void %s(%s)' % (subfunc_name,
									   ','.join(['int *%s' % arg for arg in args])))
	prefix.append('{')
	if not node is None:
		prefix.append('   assert_that( nanos6_get_cluster_node_id() == ' + str(node) + ');')
	# prefix.append('	printf("%s\\n");' % ('_' * nesting_level + subfunc_name));
	suffix = ['}']
	generate_task(all_statements, prefix, accesses, suffix, nesting_level, node)
	return subfunc_name

checknum = 0

def check(aa, subfuncs, statements):
	a = aa.array
	global use_hashing
	global checknum
	name = 'check_%d' % checknum
	checknum += 1

	# Find all regions that can be written to by the task itself
	for xofs, xsize in aa.accessible_parts('Directin'):

		# And have not already been written by the task
		for yofs, ysize in aa.unchecked_parts(xofs, xsize):

			# And if --track is being used are actually being tracked
			for ofs, size in aa.tracked_parts(yofs, ysize):

				if use_hashing:
					# Hash has smaller source files
					hashval = calc_hash(a.get_ref(ofs,size))
					statements.append('check_hash(&%s[%d], 0x%08x, %d, "%s");' % (a.name, ofs, hashval, size, name))
				else:
					# Non-hash is easier for debugging
					defn_name = generate_array('ref', a.get_ref(ofs,size))
					name_full = name + ' ' + a.name
					statements.append('check(&%s[%d], %s, %d, "%s", %d);' % (a.name, ofs, defn_name, size, name_full, ofs))
			a.set_checked(yofs, ysize, True)

def write(aa, ofs, size, subfuncs, statements):
	a = aa.array
	global use_hashing

	# Find all subregions that have not been already written by this task
	for xofs, xsize in aa.unwritten_parts(ofs, size):

		# And if --track is being used are actually being tracked
		for cofs, csize in aa.tracked_parts(xofs, xsize):
			if use_hashing:
				new_contents, inithash = generate_hash_array(csize)
				statements.append('fill(&%s[%d], 0x%08x, %d);' % (a.name, cofs, inithash, csize))
			else:
				defn_name, new_contents = generate_random_array(csize)
				statements.append('copy(&%s[%d], %s, %d);' % (a.name, cofs, defn_name, csize))

			# Update reference with the new contents
			a.update_ref(cofs, new_contents)

			# And remember that it has just been written and there is no need to check it
			a.set_written(cofs, csize, True)
			a.set_checked(cofs, csize, True)


# Generate a task
def generate_task(all_statements, task_prefix, array_accesses, task_suffix, nesting_level, use_inout = False, taskwait_and_check = False, node=None):
	global max_nesting_level
	global subfunc_number
	global max_num_tasks
	global no_taskwait
	global taskwaiton
	global if0

	subfuncs = []
	statements = []
	subtasks = 0
	tries = 0
	num_tws = 0
	max_tries = 10000

	# Always check all the in and inout regions
	for aa in array_accesses:
		if aa.accesstype in ('In', 'Inout'):
			check(aa, subfuncs, statements)

	# If at max nesting level, just write to out and inout regions
	if nesting_level >= max_nesting_level:
		for aa in array_accesses:
			if aa.accesstype in ('Out', 'Inout'):
				write(aa, aa.ofs, aa.size, subfuncs, statements)

	else:
		# Otherwise generate some random operations
		global branching
		operation_types = ['Subtask'] * 3 + ['DirectWrite']
		if not no_taskwait:
			operation_types.append('Taskwait')
		if taskwaiton:
			operation_types.append('TaskwaitOn')

		while subtasks < branching and tries < max_tries and subfunc_number < max_num_tasks:

			tries += 1

			op_type = random_structure.choice(operation_types)

			if op_type == 'Subtask':

				accesses = []
				for t in range(0,4):
					aa = random_structure.choice(array_accesses)
					a = aa.array

					# Choose an access type that is in principle OK
					if use_inout:
						accesstype = 'Inout'
					elif aa.accesstype in ('In', 'Weakin'):
						# Only Weakin and In are allowed if the parent's dependency was In or Weakin
						accesstype = random_structure.choice(['In', 'Weakin'])
					else:
						accesstype = random_structure.choice(subtask_access_types)

					ofs, size = aa.random_subaccess(accesstype)
					if ofs is None:
						continue
					assert a.child_access_valid(ofs, size, accesstype)

					# Make sure this access doesn't overlap a previous one
					end = ofs + size
					for arrayaccess in accesses:
						if arrayaccess.array == a:
							oofs = arrayaccess.ofs
							osize = arrayaccess.size
							oend = oofs + osize
							if not (oend < ofs or oofs >= end):
								ofs = max(oend,ofs)
					size = end - ofs

					if size > 0:
						accesses.append( ArrayAccess(accesstype, a, ofs, size) )

				if len(accesses) > 0:

					pragma = '#pragma oss task ' + ' '.join([make_pragma_clause(aa.array, aa.ofs, aa.size, aa.accesstype) for
															 aa in accesses ])

					if if0 and random_nodes.random() < 0.1:
						task_node = node
						pragma = pragma + ' if(0)'
					elif not nodes is None:
						task_node = random_nodes.randint(0,nodes-1)
						pragma = pragma + ' node(%d)' % task_node
					else:
						task_node = None

					# Not sure why I bothered with this: whether to inline or outline the pragma annotation
					outline = random_structure.choice([True,False])
					outline = False # node(..) clause does not work on outlined tasks
					if not outline:
						statements.append(pragma)
						pragma = None
					
					# Arrays touched by the task, which become task arguments
					arrays = sorted(set([aa.array for aa in accesses]))
					args = [a.name for a in arrays]

					# Remember parent's access levels for all regions
					for a in arrays:
						a.push_level()

					# Set access levels inside the task
					for aa in accesses:
						a = aa.array
						a.child_level(aa.ofs, aa.size, aa.accesstype)
						a.set_checked(aa.ofs, aa.size, False)
						a.set_written(aa.ofs, aa.size, False)

					# Generate the task definition
					subfunc_name = generate_subfunc(all_statements, args, accesses, pragma, nesting_level+1, task_node)

					# Call the child task from the parent task
					statements.append('%s(%s);' % (subfunc_name, ','.join(args)))
					statements.append('')

					# Set access levels in parent following the task
					for aa in accesses:
						a = aa.array
						a.set_level_after_subtask(aa.ofs, aa.size, aa.accesstype)
						a.set_checked(aa.ofs, aa.size, False)
						a.set_written(aa.ofs, aa.size, False)
					for a in arrays:
						a.pop_level()

					subtasks += 1
					use_inout = False
				
			elif op_type == 'Taskwait':

				if num_tws == 2:
					# No more than 2 consecutive taskwaits
					continue
				num_tws += 1
				statements.append('#pragma oss taskwait')
				statements.append('')

				# Read and ReadWrite data is now valid following the taskwait
				for aa in array_accesses:
					a = aa.array
					a.set_level_after_taskwait(aa.ofs, aa.size, aa.accesstype)

				# Now check all data is valid following the taskwait
				for aa in array_accesses:
					a = aa.array
					a.set_checked(aa.ofs, aa.size, False)
					a.set_written(aa.ofs, aa.size, False)
					check(aa, subfuncs, statements)

			elif op_type == 'DirectWrite':

				aa = random_structure.choice(array_accesses)
				a = aa.array

				# Only possible with Inout and Out dependencies
				if not aa.accesstype in ('Inout', 'Out'):
					continue

				ofs, size = aa.random_subaccess('Directout')
				if ofs is None:
					continue
				if not a.child_access_valid(ofs, size, 'Directout'):
					continue

				check(aa, subfuncs, statements)
				write(aa, ofs, size, subfuncs, statements)

				a.set_level_after_direct(ofs, size, 'Directout')

			elif op_type == 'TaskwaitOn':
				if num_tws == 2:
					# No more than 2 consecutive taskwaits
					continue
				num_tws += 1
				aa = random_structure.choice(array_accesses)
				a = aa.array

				ofs, size = aa.random_subaccess('NoDependency') # Any access is OK
				if ofs is None:
					continue

				statements.append('#pragma oss taskwait on(%s[%d;%d])' % (a.name, ofs, size))
				statements.append('')

				# Read and ReadWrite data is now valid following the taskwait
				a.set_level_after_taskwaiton(ofs, size, aa.accesstype)

				check(aa, subfuncs, statements)

				if a.child_access_valid(ofs, size, 'Directout'):
					write(aa, ofs, size, subfuncs, statements)
					a.set_level_after_direct(ofs, size, 'Directout')


			else:
				assert False


	if taskwait_and_check:
		statements.append('#pragma oss taskwait')
		statements.append('')
		# Read and ReadWrite data is now valid following the taskwait
		for aa in array_accesses:
			a = aa.array
			a.set_level_after_taskwait(aa.ofs, aa.size, aa.accesstype)

		# Now check all data is valid following the taskwait
		for aa in array_accesses:
			a = aa.array
			a.set_checked(aa.ofs, aa.size, False)
			a.set_written(aa.ofs, aa.size, False)
			check(aa, subfuncs, statements)

	# Define all subfunctions 
	for subfunc in subfuncs:
		all_statements.extend(subfunc)

	# Define the task itself
	all_statements.extend(task_prefix)
	for statement in statements:
		all_statements.append('    ' + statement)

		
	all_statements.extend(task_suffix)


# Generate the main function
def generate_main(prefix, suffix, taskwait_and_check):
	size0 = 1000
	size1 = 1000
	arrays = [ Array('a', size0), Array('b', size1) ]
	accesses = [ ArrayAccess('Weakinout', arrays[0], 0, size0), ArrayAccess('Inout', arrays[1], 0, size1) ]
	
	# Print the prefix (#includes, check and copy functions, etc.)
	print prefix

	# Beginning of the definition of main
	if use_dmalloc:
		# Use dmalloc and lmalloc
		prefix = """
int main(void)
{
	int *a = nanos6_dmalloc(%d * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(%d * sizeof(int));

""" % (size0, size1)

	else:
		# Use normal mallocs
		prefix = """
int main(void)
{
	int *a = (int *)malloc(%d * sizeof(int));
	int *b = (int *)malloc(%d * sizeof(int));

""" % (size0, size1)

	# Generate main and all the tasks
	all_statements = []

	# For the first task in main always use inout
	# (otherwise if --tasks is small, most of the tasks
	# will only have out dependencies)
	use_inout = False

	# Put some initial data in lmalloc array
	for access in accesses:
		if access.accesstype == 'Inout':
			# lmalloc
			tmp_statements = []
			write(access, access.ofs, access.size, None, tmp_statements)
			for s in tmp_statements:
				prefix = prefix + '    ' + s + '\n'

	if not taskwait_and_check:
		suf = '    #pragma oss taskwait'
	else:
		suf = ''

	if use_dmalloc:
		suf = suf + """
   nanos6_dfree(a, %d * sizeof(int));
   nanos6_lfree(b, %d * sizeof(int));
""" % (size0, size1)

	suffix = suf + suffix

	generate_task(all_statements, [prefix], accesses, [suffix], 0, use_inout, taskwait_and_check, 0)

	# Output all array definitions
	for line in all_arrays:
		print line

	# Output everything else
	for line in all_statements:
		print line


def main(argv):

	global use_hashing
	global max_nesting_level
	global max_num_tasks
	global use_dmalloc
	global no_taskwait
	global taskwaiton
	global nodes
	global track
	global read_outs
	global branching

	seed = 1
	try:
		opts, args = getopt.getopt( argv[1:],
									'h', ['help', 'seed=', 'no-hash', 
										  'nesting=', 'tasks=', 'serial', 
										  'no-taskwait', 'no-taskwaiton', 'nodes=', 'track=',
										  'branching='] )

	except getopt.error, msg:
		print msg
		print "for help use --help"
		sys.exit(2)
	for o, a in opts:
		if o in ('-h', '--help'):
			return Usage()
		elif o == '--seed':
			seed = hash(a)
		elif o == '--no-hash':
			use_hashing = False
		elif o == '--nesting':
			max_nesting_level = int(a)
		elif o == '--tasks':
			max_num_tasks = int(a)
		elif o == '--serial':
			use_dmalloc = False
		elif o == '--no-taskwait':
			no_taskwait = True
		elif o == '--no-taskwaiton':
			taskwaiton = False
			if0 = False
		elif o == '--nodes':
			nodes = int(a)
		elif o == '--track':
			elems = a.split(',')
			track = []
			for elem in elems:
				m = re.match('([a-z0-9_]*)\[([0-9]*)\]$', elem)
				assert m
				name = m.group(1)
				index = int(m.group(2))
				track.append( (name,index) )
		elif o == '--branching':
			branching = int(a)
		else:
			assert False
			
		if len(args) != 0:
			return Usage()

	random_structure.seed(seed)
	random_contents.seed(seed+1)
	random_nodes.seed(seed+2)

	if branching is None:
		# Calculate sensible default branching factor
		#
		# If executed with --branching b --nesting n --tasks t
		#
		# Number of tasks (which should equal t) is:
		#
		#			  2    3		  n    b^(n+1) - b
		#	 t = b + b	+ b  + ... + b	 = -----------
		#									  b-1
		#
		#		  b    n			n
		#	 t = --- (b   - 1)	>  b   - 1	  (for b>1)
		#		 b-1
		#				   1/n
		#	 so  b <  (t+1)

		n = max_nesting_level
		t = max_num_tasks
		def calc_tasks(b):
			if b == 1:
				tasks = n
			else:
				tasks = int( (math.pow(b,n+1)) - b) / (b - 1)
			# print 'tasks(%d) = %d' % (b, tasks)
			return tasks

		# Upper bound
		branching = int(math.ceil(math.pow(t+1, 1.0/n)))
		assert calc_tasks(branching) >= t

		while branching > 1:
			assert branching-1 > 0
			if calc_tasks(branching-1) < t:
				break
			branching -= 1

	t = Template(all_prefix)
	nanos6_include = '#include <nanos6.h>' if use_dmalloc else ''
	prefix = t.substitute(arg_summary='Generated by ' + ' '.join(argv), nanos6_include=nanos6_include)

	passed_print = 'printf("Test passed [$arg_summary]\\n");'

	passed_print = Template(passed_print).substitute(arg_summary= ' '.join(argv))

	t = Template("""
    $passed_print
	
    return(0);
}
	""")
	suffix = t.substitute(passed_print=passed_print)

	generate_main(prefix, suffix, taskwait_and_check = not no_taskwait)
	return 0

if __name__ == '__main__':
	sys.exit(main(sys.argv))

