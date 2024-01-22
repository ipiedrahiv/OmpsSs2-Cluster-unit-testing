#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 1;

	#pragma oss task weakinout(a[0;10]) node(1) label("1 weak")
	{
		#pragma oss task weakinout(a[0;10]) node(2) label("1.1 weak")
		{
			#pragma oss task inout(a[0;10]) node(2) label("1.1 strong")
			{
				assert_that(a[0] == 1);
				sleep(1);
				a[0] = 2;
			}
		}
	}

	#pragma oss task weakinout(a[0;10]) node(1) label("2 weak")
	{
		#pragma oss task weakinout(a[0;10]) node(2) label("2.1 weak")
		{
			#pragma oss task inout(a[0;10]) node(2) label("2.1 strong")
			{
				assert_that(a[0] == 2);
				sleep(1);
				a[0] = 3;
			}
		}
	}

	#pragma oss taskwait
	assert_that(a[0] == 3);
	printf("Finished main!\n");

    return 0;
}
