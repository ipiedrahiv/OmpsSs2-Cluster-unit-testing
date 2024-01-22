#include "util.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"
#include <numa.h>
#include <numaif.h>
#include <errno.h>
#include <sched.h>

int main(int argc, char **argv)
{
	int PAGESIZE = 4096;
	// Assume that there are two NUMA nodes
	int num_sockets = 2;
	int num_nodes = nanos6_get_num_cluster_nodes();
	int blocks_per_socket = 2;
	int blocks_per_node = num_sockets * blocks_per_socket;

	char *a = (char *)nanos6_dmalloc(PAGESIZE * blocks_per_node * num_nodes, nanos6_equpart_distribution, 0, NULL);

	int sentinel;
	for(int node = 0; node < num_nodes; node++) {
		char *start = &a[PAGESIZE*blocks_per_node*node];
		#pragma oss task weakinout(start[0;PAGESIZE*blocks_per_node]) node(node) inout(sentinel) label("weak")
		{
			for(int block=0; block < blocks_per_node; block++) {
				#pragma oss task inout(start[PAGESIZE*block;PAGESIZE]) node(node) label("check")
				{
					// On which NUMA node is the current CPU?
					int cpu = sched_getcpu();
					int numa_node = numa_node_of_cpu(cpu);
					int expect = block / blocks_per_socket;

					// Which NUMA node is the page mapped to?
					void *pages = &start[PAGESIZE*block];
					start[PAGESIZE*block] = 0; // fault it, otherwise will get -EFAULT
					int status;
					move_pages(/* pid */ 0, /* count */ 1, &pages, NULL, &status, MPOL_MF_MOVE);

					// Print out
					printf("Node %d Block %d (%p) expect NUMA node %d: CPU NUMA node: %d page NUMA node: ",
							node,
							block,
							&start[PAGESIZE*block],
							expect,
							numa_node);
					if (status >= 0) {
						printf("%d\n", status);
					} else {
						switch(status) {
							case -EACCES: printf("-EACCES\n"); break;
							case -EBUSY: printf("-EBUSY\n"); break;
							case -EFAULT: printf("-EFAULT\n"); break;
							case -EIO: printf("-EIO\n"); break;
							case -EINVAL: printf("-EINVAL\n"); break;
							case -ENOMEM: printf("-ENOMEM\n"); break;
							default:
								printf("%d\n", status);
								break;
						}
					}
					assert_that(numa_node == expect);
					assert_that(status == expect);

					sleep(1);
				}
			}
			#pragma oss taskwait
		}
	}

	#pragma oss taskwait
	nanos6_dfree(a, PAGESIZE * blocks_per_node * num_nodes);

    return 0;
}
