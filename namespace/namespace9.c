#include "util.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 1;

	#pragma oss task inout(a[0;10]) node(0) label("strong")
	{
		assert_that(a[0] == 1);
		a[0] = 2;
	}

	#pragma oss task weakinout(a[0;10]) node(1) label("weak1")
	{
		// If release access is not sent, then the location info (node 2)
		// won't be passed back to node 0.
		printf("\n\nWeak dependency not accessed at all!\n\n");
	}

	#pragma oss taskwait
	assert_that(a[0] == 2);
	printf("Finished main!\n");

    return 0;
}
