#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 1;
	int N_PER_NODE = 100;
	int numNodes = nanos6_get_num_cluster_nodes();
	int a = 0;
	printf("&a = %p\n", &a);

	#pragma oss task out(a) node(1) label("init")
	{
		a = INIT;
		usleep(10000);
	}

	#pragma oss task weakreduction(+:a) node(0) label("weakreduction0")
	{
		#pragma oss task reduction(+:a) node(0) label("reduction0")
		{
			printf("Start reduction0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
			usleep(100);
			a += 100;
			printf("Done reduction0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		}
	}

	#pragma oss task weakreduction(+:a) node(1) label("weakreduction1")
	{
		#pragma oss task reduction(+:a) node(1) label("reduction1")
		{
			printf("Start reduction1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
			usleep(100);
			a += 10000;
			printf("Done reduction1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		}
	}

	#pragma oss taskwait
	int ref = 10101;

	printf("got a = %d, expect a = %d\n", a, ref);
	assert_that(a == ref);
	printf("Done!\n");

    return 0;
}
