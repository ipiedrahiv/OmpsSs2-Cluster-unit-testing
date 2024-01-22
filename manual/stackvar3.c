#include "util.h"
#include <stdio.h>
#include "nanos6.h"
#include <unistd.h>


int main(int argc, char **argv)
{
	#pragma oss task node(1) label("task0")
	{
		int a[10];
		a[0] = 1;

		#pragma oss task inout(a[0;10]) node(0) label("task1")
		{
			assert_that(a[0] == 1);
			a[0] = 2;
		}

		#pragma oss taskwait
		assert_that(a[0] == 2);
	}
	#pragma oss taskwait

    return 0;
}
