#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int a;
	int init = 1000;
	int seq;
	a = init;
	int P = 7; // Best if it's prime
	int q = 3; // Any number coprime to P
	int *b = nanos6_lmalloc(P * sizeof(int));
	int j;

	// Make the commutative tasks ready in an unusual order
	int u = P / 3;
	for(j=0; j<P; j++) {
		#pragma oss task out(b[u]) inout(seq) label("init") node(0)
		{
			struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 1000 * 1000; // 1 ms
			nanosleep(&tim, NULL);
		}
		u = (u + q) % P;
	}

	for(j=0; j<P; j++) {
		int nodeId = j % 3;
		#pragma oss task commutative(a) in(b[j]) label("commutative0") node(nodeId)
		{
			printf("Task %d: %d => %d on %d\n", j, a, a+1, nodeId);
			a++;
		}
	}

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a == init + P);

    return 0;
}
