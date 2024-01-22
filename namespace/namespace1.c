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

	#pragma oss task inout(a[0;10]) node(1) label("task0")
	{
		printf("Execute task on %d\n", nanos6_get_cluster_node_id());
		assert_that(a[0] == 1);
		a[0] = 2;
		sleep(1);
		printf("Done execute task on %d\n", nanos6_get_cluster_node_id());
    }

	printf("Before taskwait\n");
	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a[0] == 2);

    return 0;
}
