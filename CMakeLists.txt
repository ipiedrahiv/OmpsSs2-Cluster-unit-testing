# Copyright (C) 2021  Jimmy Aguilar Mena

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required (VERSION 3.0)

if(NOT DEFINED ENV{SLURM_JOB_ID})
  message(FATAL_ERROR "configuring and running the unit tests requires a SLURM interactive session.")
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(MERCURIUM REQUIRED)

project (UnitTests)

option(WITH_ASAN "Build tests with address sanitizer" false)
option(WITH_TASKFOR "Build tests with taskfor" true)
option(WITH_TASKLOOP "Build tests with taskloop" true)
option(WITH_CONCCOMM "Build tests with commutative and concurrent" true)
option(WITH_AUTO "Build tests with auto support" false)
option(WITH_OFFLOADEDTASKDEP "Build tests with offloaded tasks dependencies support" true)
option(WITH_NUMA "Build tests for NUMA awareness" true)
option(WITH_MALLEABILITY "Build tests with malleability" false)
option(WITH_SERIALIZE "Build tests with malleability" false)
option(WITH_NEWAPI "Build tests for new API" false)
option(WITH_REDUCTIONS "Build tests for reductions" false)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CTEST_OUTPUT_ON_FAILURE ON)
cmake_policy(SET CMP0074 NEW)

# Auxiliar function to declare mpi tests
# Extra named arguments are:
# CORES: Passed to srun in -C parameter
# SUFFIX: Optional suffix for the test name
# NODES: List of number of nodes.
# PROPERTIES: for set_tests_properties
function (add_mpi_tests exec)
  cmake_parse_arguments(MPITEST "" "CORES;SUFFIX" "NODES;PROPERTIES" ${ARGN})

  # If address sanitizer is enabled concat to the ENV
  if (ASAN_ENV)
    list(APPEND MPITEST_ENVIRONMENT ${ASAN_ENV})
  endif ()

  foreach (nodes ${MPITEST_NODES})
    set(testname ${exec}${MPITEST_SUFFIX}_${nodes})

    if (MPITEST_CORES) # Set the number of cores from user.
      set(cores ${MPITEST_CORES})
    else() # Calculate the number of cores / test. Use the maximum possible
      math(EXPR cores "$ENV{SLURM_JOB_NUM_NODES} * $ENV{SLURM_CPUS_ON_NODE} / ${nodes}")
      if ($ENV{SLURM_CPUS_ON_NODE} LESS cores)
	set(cores $ENV{SLURM_CPUS_ON_NODE})
      endif ()
    endif ()

    # Add the test
    add_test(NAME ${testname} COMMAND srun --cpu-bind=cores -n ${nodes} -c ${cores} ./${exec})
    message(" Add test:  ${testname}")

    # Set other properties.
    set_tests_properties(${testname} PROPERTIES ${MPITEST_PROPERTIES})
  endforeach ()
endfunction ()

# Add tests based on file list receives the sources list and all other
# arguments are passed directly to add_mpi_tests
function (add_mpi_tests_list test_sources)

  cmake_parse_arguments(MPITESTLIST "" "" "PROPERTIES" ${ARGN})

  # Tests for varying numbers of nodes
  foreach (source ${${test_sources}})
    string(REGEX REPLACE "${PROJECT_SOURCE_DIR}/(.*)" "\\1" subfile ${source})
    string(REPLACE "/" "_" testname ${subfile})
    get_filename_component(exefile ${testname} NAME_WE)  # delete path and extension
    message("Add executable: ${subfile} => ${exefile}")

    add_executable(${exefile} ${subfile} $<TARGET_OBJECTS:util_object>)
    target_link_libraries(${exefile} PRIVATE Threads::Threads)

    add_mpi_tests(${exefile}
      ${MPITESTLIST_UNPARSED_ARGUMENTS}
      PROPERTIES TIMEOUT 30 ${MPITESTLIST_PROPERTIES})

  endforeach ()
endfunction ()

# Add tests based on file list
function (add_malleable_tests_list test_sources)

  # Tests for varying numbers of nodes
  foreach (source ${${test_sources}})
    string(REGEX REPLACE "${PROJECT_SOURCE_DIR}/(.*)" "\\1" subfile ${source})

    string(REGEX REPLACE  "^.+_([1-9])_[1-9].c$" "\\1" num_min_nodes ${subfile})
    string(REGEX REPLACE  "^.+_[1-9]_([1-9]).c$" "\\1" num_max_nodes ${subfile})

    string(REPLACE "/" "_" testname ${subfile})
    get_filename_component(exefile ${testname} NAME_WE)  # delete path and extension
    message("Add executable: ${subfile} => ${exefile}")

    add_executable(${exefile} ${subfile} $<TARGET_OBJECTS:util_object>)
    target_link_libraries(${exefile} PRIVATE Threads::Threads)

    add_mpi_tests(${exefile}
      CORES 6
      NODES "${num_min_nodes}"
      PROPERTIES TIMEOUT 60
      ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.num_max_nodes=${num_max_nodes}" )

    # Add extra test with eager policy only for shrink benchmarks
    # (have shrink in the name)
    string(FIND ${exefile} "shrink" INDEX)

    if (INDEX GREATER -1)
      add_mpi_tests(${exefile}
	CORES 6
	NODES "${num_min_nodes}"
	SUFFIX "_eager"
	PROPERTIES TIMEOUT 60
	ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.num_max_nodes=${num_max_nodes},cluster.default_shrink_transfer_policy=eager" )
    endif ()

  endforeach ()
endfunction ()

#Compilation flags
set(default_build_type "Debug")
add_compile_definitions(_GNU_SOURCE)
add_compile_options(-O0 -ggdb -g --ompss-2 -k)
add_link_options(--ompss-2)

add_library(util_object OBJECT util.c)

# Enable address sanitizer
if (WITH_ASAN)
  message("Using ASAN")
  add_compile_options(-fno-omit-frame-pointer -fsanitize=address)

  find_library(LIBASAN
    NAMES asan
    HINTS ENV LIBRARY_PATH LD_LIBRARY_PATH)

  set(ASAN_ENV "LD_PRELOAD=${LIBASAN}"
    "ASAN_OPTIONS=detect_leaks=0:check_initialization_order=1:strict_init_order=1")

  add_link_options(-fno-omit-frame-pointer -fsanitize=address)
endif ()

include_directories(${PROJECT_SOURCE_DIR})

#Add the unit tests
enable_testing()

# Tests specifically on many ranks
file(GLOB test_sources_all manual/*.c)
add_mpi_tests_list(test_sources_all NODES 1 2 3 11 12)

# If we add more numeric specific tests we can use a loop for this.
# But for now this is enough.

file(GLOB test_sources_2 nasty/test2_*.c)
add_mpi_tests_list(test_sources_2 NODES 2)

file(GLOB test_sources_3 nasty/test3_*.c)
add_mpi_tests_list(test_sources_3 NODES 3)

file(GLOB test_sources_4 nasty/test4_*.c)
add_mpi_tests_list(test_sources_4 NODES 4)

# Namespace test (unconditional)
file(GLOB test_namespace namespace/*[!_][!*].c)
add_mpi_tests_list(test_namespace NODES 3)

file(GLOB test_namespace namespace/*_4.c)
add_mpi_tests_list(test_namespace NODES 4)
set_tests_properties(namespace_no_taskwait1_3
  PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:0: main: subtask accesses stack after task completion ")
set_tests_properties(namespace_no_taskwait2_3
  PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:0: main: lfree or dfree without preceding taskwait")
set_tests_properties(namespace_no_taskwait4_3
  PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:[0-9]: func0: subtask accesses stack after task completion ")
set_tests_properties(namespace_no_taskwait5_3
  PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:[0-9]: func0: subtask accesses stack after task completion ")
set_tests_properties(namespace_no_taskwait6_3
  PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:0: main: subtask accesses stack after task completion ")

# API tests (unconditional)
if (WITH_NEWAPI)
  file(GLOB test_api apichecks/*.c)
  add_mpi_tests_list(test_api NODES 1 2)

  ## Leader thread
  set_tests_properties(apichecks_reserve_leader_thread_1
    PROPERTIES PASS_REGULAR_EXPRESSION "Expected: 0")
  set_tests_properties(apichecks_reserve_leader_thread_2
    PROPERTIES PASS_REGULAR_EXPRESSION "Expected: 1")

  add_mpi_tests(apichecks_reserve_leader_thread
    NODES 2
    SUFFIX "_noreserve"
    PROPERTIES TIMEOUT 30
    ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cpumanager.reserve_cpu_leader=false"
    PASS_REGULAR_EXPRESSION "Expected: 0")

  ## Group messages
  set_tests_properties(apichecks_group_messages_2
    PROPERTIES PASS_REGULAR_EXPRESSION "Expected: 1")

  add_mpi_tests(apichecks_group_messages
    NODES 2
    SUFFIX "_nogroup"
    PROPERTIES TIMEOUT 30
    ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.group_messages=false"
    PASS_REGULAR_EXPRESSION "Expected: 0")
endif ()

# Option tests (conditional)
if (WITH_TASKFOR)
  file(GLOB test_sources_3 taskfor/test3_*.c)
  add_mpi_tests_list(test_sources_3 NODES 3)

  file(GLOB test_sources_4 taskfor/test4_*.c)
  add_mpi_tests_list(test_sources_4 NODES 4)
  set_tests_properties(taskfor_test3_taskfor_empty_3
    PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:0: Task for taskfor has no iterations: currently not supported")
endif ()

if (WITH_TASKLOOP)
  file(GLOB test_sources_3 taskloop/test3_*.c)
  add_mpi_tests_list(test_sources_3 NODES 3)
  file(GLOB test_sources_4 taskloop/test4_*.c)
  add_mpi_tests_list(test_sources_4 NODES 4)
endif ()

if (WITH_CONCCOMM)
  file(GLOB test_conccomm conccomm/*.c)
  add_mpi_tests_list(test_conccomm NODES 3)
  set_tests_properties(conccomm_concurrent12_3
    PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:[0-9]: Warning: Weak access type readwrite nested inside access type concurrent for task b is not supported")
  set_tests_properties(conccomm_concurrent13_3
    PROPERTIES PASS_REGULAR_EXPRESSION "Error: Node:[0-9]: Warning: Weak access type readwrite nested inside access type concurrent for task b is not supported")
endif ()

if (WITH_NUMA)
  file(GLOB test_numa numa/*.c)
  add_link_options(-lnuma)
  add_mpi_tests_list(test_numa NODES 2)
endif ()


if (WITH_ALLMEMORY)
  file(GLOB test_allmemory allmemory/*.c)
  add_mpi_tests_list(test_allmemory NODES 3
    PROPERTIES ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.eager_weak_fetch=false,cluster.message_max_size=-1")
endif ()

if (WITH_OFFLOADEDTASKDEP)
  file(GLOB test_offloadedTaskDeps offloadedTaskDeps/*.c)
  add_mpi_tests_list(test_offloadedTaskDeps NODES 2)
endif ()

if (WITH_MALLEABILITY)
  file(GLOB test_malleability malleability/*.c)
  add_malleable_tests_list(test_malleability)

  # Assert we effectively Destruct BEFORE Wakeup so not hangs at the
  # MPI_Finalize
  set_tests_properties(malleability_test_spawn1_shrink1_1_2_1
    PROPERTIES PASS_REGULAR_EXPRESSION "Destructing\nWakeup\nDestructing")

  # Test byone spawn policy
  set_property(TEST malleability_test_spawn2_shrink11_1_3_1 APPEND
    PROPERTY ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.num_max_nodes=3,cluster.default_spawn_policy=single")

  set_property(TEST  malleability_test_spawn2_shrink11_1_3_eager_1 APPEND
    PROPERTY ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.num_max_nodes=3,cluster.default_spawn_policy=single,cluster.default_shrink_transfer_policy=eager")
endif ()

if (WITH_SERIALIZE)
   file(GLOB test_serialize serialize/serialize*.c)
   file(GLOB test_deserialize serialize/deserialize*.c)

   add_mpi_tests_list(test_serialize NODES 4)

   # Fixtures setups are to enforce order. Checkpoints need to be
   # executed before restart otherwise restart will fail.
   set_tests_properties(serialize_serialize_4 PROPERTIES FIXTURES_SETUP SERIALIZED)
   set_tests_properties(serialize_serialize2_4 PROPERTIES FIXTURES_SETUP SERIALIZED2)
   set_tests_properties(serialize_serializeN_4 PROPERTIES FIXTURES_SETUP SERIALIZEDN)

   # 1 to check the self communicator works
   # 3 because 512 * 4 / 3 has a remaining (extra region)
   # 4 for perfect match
   add_mpi_tests_list(test_deserialize NODES 1 3 4)
   foreach(x 1 3 4)
     set_tests_properties(serialize_deserialize_${x} PROPERTIES FIXTURES_REQUIRED SERIALIZED)
     set_tests_properties(serialize_deserialize2_${x} PROPERTIES FIXTURES_REQUIRED SERIALIZED2)
     set_tests_properties(serialize_deserializeN_${x} PROPERTIES FIXTURES_REQUIRED SERIALIZEDN)
   endforeach()
endif ()

if (WITH_REDUCTIONS)
  file(GLOB test_reductions reductions/*.c)
  add_mpi_tests_list(test_reductions NODES 3)
endif ()

file(GLOB test_nocomm commdisable/*.c)
add_mpi_tests_list(test_nocomm
  NODES 1 2 4
  PROPERTIES ENVIRONMENT "NANOS6_CONFIG_OVERRIDE=cluster.communication=disabled" )

#add the install targets
configure_file(nanos6.toml nanos6.toml)

configure_file(trace.sh trace.sh @ONLY)
configure_file(extrae_template.xml extrae.xml @ONLY)
