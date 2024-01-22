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
			sleep(3);
			a[0] = 2;
			printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}
	#pragma oss task weakinout(a[0;10]) node(1) label("weak2")
	{
		#pragma oss task inout(a[0;10]) node(1) label("strong2")
		{
			printf("\n\nStart task B on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 2);
			sleep(3);
			a[0] = 3;
			printf("\n\nDone task B on %d\n", nanos6_get_cluster_node_id());
		}
	}

	#pragma oss task inout(a[0;10]) node(0) label("last")
	{
		printf("\n\nLast task C on %d\n\n", nanos6_get_cluster_node_id());
		assert_that(a[0] == 3);
		a[0] = 4;
	}

	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a[0] == 4);

    return 0;
}
