#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int N = 1;
	int *a = nanos6_lmalloc(N * sizeof(int));
	int j;

	// Initial values
	for(j=0; j<N; j++) {
		a[j] = 1000+j;
	}

	for(j = 0; j < N; j++) {
		int nodeId = 1;

		#pragma oss task weakconcurrent(a[0;N]) label("concurrent") node(nodeId)
		{
			#pragma oss task weakconcurrent(a[0;N]) label("concurrent") node(nodeId)
			{
				#pragma oss task inout(a[j]) node(nodeId) label("task0")
				{
					assert_that(a[j] == 1000+j);
					a[j] = 2000+j;
				}
			}
			// #pragma oss taskwait
		}
	}

	#pragma oss taskwait
	for(j=0; j<N; j++) {
		printf("%4d %4d\n", j, a[j]);
	}
	for(j=0; j<N; j++) {
		assert_that(a[j] == 2000+j);
	}
    return 0;
}
