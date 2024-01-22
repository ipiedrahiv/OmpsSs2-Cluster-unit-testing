#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool test_stack()
{
	/* 'a' should be in the stack of the
	 * WorkerThread that calls test_stack */
	int a;

	#pragma oss task out(a) label("init")
	a = 0;

	#pragma oss task inout(a) label("inc1")
	a++;

	#pragma oss task inout(a) label("inc2")
	a++;

	#pragma oss task in(a) label("task0")
	printf("a: %d\n", a);

	#pragma oss taskwait

	return (a == 2);
}


int main(int argc, char *argv)
{
	test_t test = FUNC_DEF(test_stack);

	return run_test(&test);
}
