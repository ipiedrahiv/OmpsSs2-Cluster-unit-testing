#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int INIT = 123;
	int N = 2;
	int a = INIT;
	printf("&a = %p\n", &a);

	#pragma oss task reduction(+:a) node(1) label("reduction0")
	{
		printf("Start task0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		usleep(1000);
		a++;
		printf("Done task0 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
	}

	#pragma oss task reduction(+:a) node(1) label("reduction1")
	{
		printf("Start task1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		usleep(1000);
		a++;
		printf("Done task1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
	}

	#pragma oss task in(a) node(1)
	{
		int expected = INIT + N;
		printf("on node %d got a = %d, expect a = %d\n", nanos6_get_cluster_node_id(), a, expected);
		assert_that(a == expected);
	}

	#pragma oss taskwait
	printf("Done!\n");

    return 0;
}
