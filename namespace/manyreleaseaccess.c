#include "util.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int n = 1000;
	int nodes = nanos6_get_num_cluster_nodes();
    int *a = nanos6_dmalloc(n*10*sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int k;
    struct timeval time_start, time_end;  // For timing each iteration

	gettimeofday(&time_start, NULL);
    for(k = 0; k<n; k++)
    {
        #pragma oss task weakout(a[k*10;10]) node(k % nodes) label("task0")
        {
            // printf("write a[%d] on %d\n", k, nanos6_get_cluster_node_id());
			#pragma oss task out(a[k*10]) out(a[k*10+1]) out(a[k*10+2]) out(a[k*10+3]) out(a[k*10+4]) \
			                 out(a[k*10+5]) out(a[k*10+6]) out(a[k*10+7]) out(a[k*10+8]) out(a[k*10+9]) label("task1")
			{
			}
        }
    }
	#pragma oss taskwait
	gettimeofday(&time_end, NULL);
	double secs = (time_end.tv_sec - time_start.tv_sec) + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
	printf("%3.2f sec\n", secs);

    return 0;
}
