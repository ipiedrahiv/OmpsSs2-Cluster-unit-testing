#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int P = 3; // Best if it's prime
	int q = 2; // Any number coprime to P
	int *a = nanos6_lmalloc(P * sizeof(int));
	int *b = nanos6_lmalloc(P * sizeof(int));
	int j;
	int seq;
	printf("a: %p b: %p\n", a, b);

	// Initial values
	for(j=0; j<P; j++) {
		a[j] = 1000+j;
	}

	// Make the concurrent tasks ready in an unusual order
	int u = P / 3;
	for(j=0; j<P; j++) {
		int nodeId = j % 3;
		#pragma oss task out(b[u]) inout(seq) label("init") node(0)
		{
			struct timespec tim;
			tim.tv_sec = 0;
			tim.tv_nsec = 1000 * 1000; // 1 ms
			nanosleep(&tim, NULL);
			b[u] = 2000+u;
		}
		u = (u + q) % P;
	}

	j = 0;
	int nodeId = j % 3;
	#pragma oss task concurrent(b[j]) weakconcurrent(a[0;P]) label("concurrent0") node(nodeId)
	{
		printf("Start task %d on %d: b[%d] = %d\n", j, nanos6_get_cluster_node_id(), j, b[j]);
		assert_that(b[j] == 2000+j);
		b[j] = 3000+j; // output value

		struct timespec tim;
		tim.tv_sec = 0;
		tim.tv_nsec = 100 * 1000 * 1000; // 100 ms
		nanosleep(&tim, NULL);

		printf("Done task %d on %d\n", j, nanos6_get_cluster_node_id());
	}

	j = 1;
	nodeId = j % 3;
	#pragma oss task concurrent(b[j]) weakconcurrent(a[0;P]) label("concurrent1") node(nodeId)
	{
		printf("Start task %d on %d: b[%d] = %d\n", j, nanos6_get_cluster_node_id(), j, b[j]);
		assert_that(b[j] == 2000+j);
		b[j] = 3000+j; // output value

		struct timespec tim;
		tim.tv_sec = 0;
		tim.tv_nsec = 100 * 1000 * 1000; // 100 ms
		nanosleep(&tim, NULL);

		#pragma oss task out(a[j]) label("strong1") node(nodeId)
		{
			a[j] = 4000+j; // output value
		}
		printf("Done task %d on %d\n", j, nanos6_get_cluster_node_id());
	}

	// j = 2;
	// nodeId = j % 3;
	// #pragma oss task concurrent(b[j]) weakconcurrent(a[0;P]) label("concurrent2") node(nodeId)
	// {
	// 	printf("Start task %d on %d: b[%d] = %d\n", j, nanos6_get_cluster_node_id(), j, b[j]);
	// 	assert_that(b[j] == 2000+j);
	// 	b[j] = 3000+j; // output value

	// 	struct timespec tim;
	// 	tim.tv_sec = 0;
	// 	tim.tv_nsec = 100 * 1000 * 1000; // 100 ms
	// 	nanosleep(&tim, NULL);

	// 	#pragma oss task out(a[j]) label("strong2") node(nodeId)
	// 	{
	// 		a[j] = 4000+j; // output value
	// 	}
	// 	printf("Done task %d on %d\n", j, nanos6_get_cluster_node_id());
	// }

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	printf("%4d %4d %4d\n", j, a[1], b[1]);
	sleep(2);
	assert_that(a[1] == 4000+j);
	assert_that(b[1] == 3000+j);
    return 0;
}
