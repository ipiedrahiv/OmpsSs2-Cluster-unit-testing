#include "util.h"
#include <stdio.h>
#include "nanos6.h"
#include <unistd.h>


int main(int argc, char **argv)
{
	int a[10];
	a[0] = 1;

	#pragma oss task in(a[0;10]) node(1) label("task0")
	{
		assert_that(a[0] == 1);
    }

	#pragma oss taskwait
	assert_that(a[0] == 1);

    return 0;
}
