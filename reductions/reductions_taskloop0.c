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

	#pragma oss taskloop reduction(+:a) label("reduction")
	for(int j = 0; j < N; j++) {
		int inc = j*j;
		// printf("Start inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
		usleep(1000);
		a += inc;
		// printf("Done inc by %d on %d: a = %d [%p]\n", inc, nanos6_get_cluster_node_id(), a, &a);
	}

	#pragma oss taskwait
	int ref = INIT;
	for(int j = 0; j < N; j++) {
		ref += j*j;
	}

	printf("got a = %d, expect a = %d\n", a, ref);
	assert_that(a == ref);
	printf("Done!\n");

    return 0;
}
