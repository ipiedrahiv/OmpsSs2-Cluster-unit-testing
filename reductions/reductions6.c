#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 0;
	int N = 20;
	int a = INIT;
	printf("&a = %p\n", &a);

	for(int i=0; i<N; i++) {
		int nodeNum = 1;
		// #pragma oss task reduction(+:a) node(i%3) label("reduction")
		#pragma oss task reduction(+:a) node(nodeNum) label("reduction")
		{
			// printf("Start task0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
			usleep(100); //000 * (N-i));
			a += 1 <<i;
			// printf("Done task0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		}
	}

	#pragma oss task in(a) node(0)
	{
		int expected = (1 << N) - 1;
		printf("on node %d got a = %x, expect a = %x\n", nanos6_get_cluster_node_id(), a, expected);
		assert_that(a == expected);
	}

	#pragma oss taskwait
	printf("Done!\n");

    return 0;
}
