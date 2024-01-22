#!/bin/bash


tests=(test_location test_flush test_if0 test_wait)
nodes=(1 2 3 15 16)

function run_test()
{
	_test=$1
	echo "Testing $1"

	for _nodes_nr in ${nodes[@]}; do
		echo "Number of nodes:$_nodes_nr"
		mpirun -np $_nodes_nr taskset -c 0-7 ./$1 2> log/"${_test}_${_nodes_nr}.out"
	done
}


#load nanos6 environment
. nanos6_env.sh

echo "SCHEDULER: $NANOS6_SCHEDULER"
echo "RUNTIME: $NANOS6"
mkdir -p log

for _test in test_* ; do
	run_test $_test
done
