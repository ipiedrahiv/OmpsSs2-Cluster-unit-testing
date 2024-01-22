#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
	int a[2] = {100, 200};
	int b[2] = {300, 400};
	int N = 10;
	int j;

	printf("a: %p  b: %p\n", a, b);
	// #pragma oss task out(a[0]) label("wait") node(0)
	// {
	// 	sleep(1);
	// 	printf("Done wait on a[0]\n");
	// 	a[0] = 5;
	// }

	#pragma oss task concurrent(b[0]) weakconcurrent(a[0;2]) label("left") node(1)
	{
		printf("Start left on %d\n", nanos6_get_cluster_node_id());
		b[0] = 10;
		a[1] = 1000; // should be ignored
		sleep(1);
		#pragma oss task out(a[0]) label("left-write") node(1)
		{
			a[0] = 5;
		}
		printf("Done left on %d\n", nanos6_get_cluster_node_id());
	}

	#pragma oss task concurrent(b[1]) weakconcurrent(a[0;2]) label("right") node(2)
	{
		printf("Start right on %d\n", nanos6_get_cluster_node_id());
		b[1] = 11;
		a[0] = 1000; // should be ignored
		sleep(1);
		#pragma oss task out(a[1]) label("right-write") node(2)
		{
			a[1] = 6;
		}
		printf("Done right on %d\n", nanos6_get_cluster_node_id());
	}

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait %d(5) %d(6) %d(10) %d(11)\n", a[0], a[1], b[0], b[1]);
	if (a[0] == 5 && a[1] == 6 && b[0] == 10 && b[1] == 11) {
	} else {
		sleep(5);
	}

	assert_that(a[0] == 5);
	assert_that(a[1] == 6);
	assert_that(b[0] == 10);
	assert_that(b[1] == 11);
    return 0;
}
