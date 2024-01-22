#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 123;
	int N = 100;
	int a = INIT;
	printf("&a = %p\n", &a);

	for(int i=0; i<N; i++) {
		int nodeNum = i % 2;
		// #pragma oss task reduction(+:a) node(nodeNum) label("reduction")
		#pragma oss task reduction(+:a) node(nodeNum) label("reduction")
		{
			// printf("Start inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
			usleep(100); //000 * (N-i));
			a ++;
			// printf("Done inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
		}
	}

	#pragma oss task in(a) node(0)
	{
		int expected = INIT + N;
		// printf("on node %d got a = %d, expect a = %d\n", nanos6_get_cluster_node_id(), a, expected);
		assert_that(a == expected);
	}

	#pragma oss taskwait
	printf("Done!\n");

    return 0;
}
