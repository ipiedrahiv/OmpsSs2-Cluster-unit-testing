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
		#pragma oss task weakinout(a[0;10]) node(2) label("weak2")
		{
			#pragma oss task inout(a[0;10]) node(0) label("strong3")
			{
				printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
				assert_that(a[0] == 1);
				sleep(1);
				a[0] = 2;
				printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
			}
		}
	}

	#pragma oss taskwait
	assert_that(a[0] == 2);

    return 0;
}
