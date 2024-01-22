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

	#pragma oss task weakin(a[0;10]) node(1) label("weak1")
	{
		#pragma oss task in(a[0;10]) node(1) label("strong1")
		{
			printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 1);
			sleep(3);
			printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}

#if 0
	#pragma oss task in(a[0;10]) node(0) label("last")
	{
		printf("\n\nLast task C on %d\n\n", nanos6_get_cluster_node_id());
		assert_that(a[0] == 1);
	}
#endif

	#pragma oss taskwait
	printf("After taskwait\n");
	assert_that(a[0] == 1);

    return 0;
}
