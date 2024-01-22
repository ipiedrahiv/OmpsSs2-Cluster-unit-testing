Readme
======

This is the set of development tests for the Ompss-2@Clusters

Pre-installation
----------------

To run the tests it is needed to load the environment first. There is
a file **nanos6_env.sh** with a basic configuration and modules
needed. (The versions of the modules change according to the
user installation; but this is a working combination)

You should not run the tests in an login node. It is recommended to
use an interactive node:

In Marenostrum it is:

```
salloc -q debug -c 48 -n 1 -t 02:00:00
```

Installation
------------

1. Clone the repository
	```
	git clone `repo_url` directory
	cd directory
	```
2. Create build directory
	```
	mkdir build
	cd build
	```
3. Generate build files and compile
	```
	cmake ..
	make
	``` 
4. Run the tests
	```
	make check
	```

Create a test
-------------

The easiest method to create a test is to create a C source file in
the root directory of the project with the name:
test_<name_of_the_test>.c and let CMake do the rest.

The basic source for a test file should be:

```c
#include "util.h"

/* Return true on success false on error. */
bool my_test_function()
{
	if (error)
		return false;

	return true;
}


int main()
{
	test_t test = FUNC_DEF(my_test_function);

	return run_test(&test);
}

```

In the `util.h` file are some predefined functions you can use to
follow a common schema. (specially the {l,d}{malloc,free} wrappers)

After adding a new file to the sources clear the **build** directory
(inside the **build** directory `rm -rf *`) to force cmake search all
the source files again. Then repeat the installation steps starting on
step 3.

