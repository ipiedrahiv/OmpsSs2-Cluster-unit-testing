#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int a;
	a = 123;
	int N = 2;
	int j;

	for(j=0; j<N; j++) {
		int nodeId = j % 2;
		#pragma oss task concurrent(a) label("concurrent") node(nodeId)
		{
			printf("Start task %d on %d\n", j, nanos6_get_cluster_node_id());
			struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 1000 * 1000; // 1 ms
			nanosleep(&tim, NULL);
			printf("Done task %d on %d\n", j, nanos6_get_cluster_node_id());
		}
	}

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a == 123);

    return 0;
}
