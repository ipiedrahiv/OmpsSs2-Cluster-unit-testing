#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 1;

	#pragma oss task weakinout(a[0;10]) node(1) label("weak1")
	{
		#pragma oss task inout(a[0;10]) node(1) label("strong1")
		{
			printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 1);
			a[0] = 2;
			sleep(2);
			printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}
	#pragma oss task weakinout(a[0;10]) node(2) label("weak2")
	{
		#pragma oss task inout(a[0;10]) node(2) label("strong2")
		{
			printf("\n\nStart task B on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 2);
			a[0] = 3;
			sleep(1);
			printf("\n\nDone task B on %d\n", nanos6_get_cluster_node_id());
		}
	}

	sleep(1);
	#pragma oss task weakinout(a[0;10]) node(1) label("weak3")
	{
		#pragma oss task inout(a[0;10]) node(1) label("strong3")
		{
			printf("\n\nStart task C on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 3);
			a[0] = 4;
			sleep(1);
			printf("\n\nDone task C on %d\n", nanos6_get_cluster_node_id());
		}
	}

	sleep(1);
	#pragma oss task weakinout(a[0;10]) node(2) label("weak4")
	{
		#pragma oss task inout(a[0;10]) node(2) label("strong4")
		{
			printf("\n\nStart task D on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 4);
			a[0] = 5;
			sleep(1);
			printf("\n\nDone task D on %d\n", nanos6_get_cluster_node_id());
		}
	}

	sleep(1);
	#pragma oss task weakinout(a[0;10]) node(1) label("weak5")
	{
		#pragma oss task inout(a[0;10]) node(1) label("strong5")
		{
			printf("\n\nStart task E on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 5);
			a[0] = 6;
			sleep(1);
			printf("\n\nDone task E on %d\n", nanos6_get_cluster_node_id());
		}
	}

	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a[0] == 6);

    return 0;
}
