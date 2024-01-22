#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int data[12];
	#pragma oss task weakinout(data[0;12]) label("A") node(0)
	{
		int *x = nanos6_lmalloc(sizeof(int));
		printf("x: %p\n", x);
		#pragma oss task out(x[0]) label("B") node(1)
		{}

		#pragma oss task inout(x[0]) label("B") node(1)
		{
			nanos6_lfree(x, sizeof(int));
		}

	}
    
	#pragma oss taskwait
	printf("Test passed\n");
    return 0;
}
