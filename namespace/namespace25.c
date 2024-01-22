#include "util.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));

	#pragma oss task weakout(a[0;10]) node(1) label("weak1")
	{
		#pragma oss task out(a[0;10]) node(2) label("strong1")
		{
			printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
			sleep(1);
			a[0] = 2;
			printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}

	#pragma oss task weakout(a[0;10]) node(1) label("weak2")
	{
	}

	#pragma oss taskwait

    return 0;
}
