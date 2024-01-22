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

	#pragma oss task out(a) node(1) label("wait")
	{
		a = 2;
    }

	#pragma oss task reduction(+:a) node(2) label("task1")
	{
		printf("Start task1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
		usleep(10000);
		a++;
		printf("Done task1 on %d: a = %d [%p]\n", nanos6_get_cluster_node_id(), a, &a);
    }

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait: a = %d\n", a);
	assert_that(a == 3);

    return 0;
}
