#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int P = 103; // Best if it's prime
	int q = 70; // Any number coprime to P
	int *a = nanos6_lmalloc(P * sizeof(int));
	int *b = nanos6_lmalloc(P * sizeof(int));
	int j;
	int seq;

	// Initial values
	for(j=0; j<P; j++) {
		a[j] = 1000+j;
	}

	// Make the concurrent tasks ready in an unusual order
	int u = P / 3;
	for(j=0; j<P; j++) {
		int nodeId = j % 3;
		#pragma oss task out(b[u]) inout(seq) label("init") node(nodeId)
		{
			struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 1000 * 1000; // 1 ms
			nanosleep(&tim, NULL);
			b[u] = 2000+u;
		}
		u = (u + q) % P;
	}

	for(j = 0; j < P; j++) {
		int nodeId = j % 3;

		#pragma oss task concurrent(b[j]) weakconcurrent(a[0;P]) label("concurrent") node(nodeId)
		{
			assert_that(b[j] == 2000+j);
			b[j] = 3000+j; // output value

			struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 1 * 1000 * 1000; // 1 ms
			nanosleep(&tim, NULL);

			int offloadNode = (j/2) % 3;
			#pragma oss task out(a[j]) label("strong") node(offloadNode)
			{
				a[j] = 4000+j; // output value
			}
		}
	}

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	for(j=0; j<P; j++) {
		printf("%4d %4d %4d\n", j, a[j], b[j]);
	}
	for(j=0; j<P; j++) {
		assert_that(a[j] == 4000+j);
		assert_that(b[j] == 3000+j);
	}
    return 0;
}
