#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int a;
	int init = 0;
	a = init;
	int N = 100;
	int j;

	for(j=0; j<N; j++) {
		int nodeId = j % 2;
		#pragma oss task commutative(a) label("commutative0") node(nodeId)
		{
			printf("Task %d: %d => %d on %d\n", j, a, a+1, nodeId);
			a++;
		}
	}

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a == init + N);

    return 0;
}
