#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 123;
	int a = INIT;
	printf("&a = %p\n", &a);

	#pragma oss task weakreduction(+:a) node(1) label("weakreduction") nowait
	{
		#pragma oss task reduction(+:a) node(1) label("reduction")
		{
			// printf("Start\n");
			a ++;
			usleep(10000);
			// printf("Done\n");
		}
		#pragma oss taskwait
	}

	#pragma oss taskwait
	int ref = INIT + 1;

	printf("got a = %d, expect a = %d\n", a, ref);
	assert_that(a == ref);
	printf("Done!\n");

    return 0;
}
