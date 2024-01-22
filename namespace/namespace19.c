#include "util.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 100;

	int niter = 10;
	for(int i=0; i<niter; i++) {

		#pragma oss task inout(a[0;10]) node(1) label("strong1")
		{
			// printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 100 + i);
			a[0] = 100 + (i+1);
			// printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}

	#pragma oss taskwait
	assert_that(a[0] == 100 + niter);

    return 0;
}
