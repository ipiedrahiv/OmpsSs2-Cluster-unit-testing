#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 123;
	int N_PER_NODE = 100;
	int numNodes = nanos6_get_num_cluster_nodes();
	int a = INIT;
	printf("&a = %p\n", &a);

	for(int nodeNum = 0; nodeNum < numNodes; nodeNum++) {
		#pragma oss task weakreduction(+:a) node(nodeNum) label("weakreduction")
		for(int i = 0; i < N_PER_NODE; i++) {
			#pragma oss task reduction(+:a) node(nodeNum) label("reduction")
			{
				int j = nodeNum * N_PER_NODE + i;
				int inc = j*j;
				printf("Start inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
				usleep(100);
				a += inc;
				printf("Done inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
			}
		}
	}

	#pragma oss taskwait
	int ref = INIT;
	for(int j = 0; j < numNodes * N_PER_NODE; j++) {
		ref += j*j;
	}

	printf("got a = %d, expect a = %d\n", a, ref);
	assert_that(a == ref);
	printf("Done!\n");

    return 0;
}
