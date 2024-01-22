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
	int numNodes = 2;
	int a = 0;
	printf("&a = %p\n", &a);

	#pragma oss task out(a) node(2)
	{
		a = INIT;
	}

	for(int nodeNum = 0; nodeNum < numNodes; nodeNum++) {
		// #pragma oss task weakreduction(+:a) node(nodeNum) label("weakreduction")
		for(int i = 0; i < N_PER_NODE; i++) {
			#pragma oss task reduction(+:a) node(nodeNum) label("reduction")
			{
				int j = nodeNum * N_PER_NODE + i;
				int inc = j*j;
				usleep(100);
				a += inc;
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
