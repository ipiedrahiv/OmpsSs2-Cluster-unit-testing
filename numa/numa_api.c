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

	for(int blocks_per_socket = 2; blocks_per_socket < 23; blocks_per_socket++) {
		printf("blocks_per_socket: %d\n", blocks_per_socket);

		int blocks_per_node = num_sockets * blocks_per_socket;

		nanos6_bitmask_t bitmask;
		nanos6_bitmask_set_wildcard(&bitmask, NUMA_ANY_ACTIVE);

		char *x = nanos6_lmalloc(PAGESIZE * blocks_per_node);
		char *a = (char *)nanos6_numa_alloc_block_interleave(PAGESIZE * blocks_per_node, &bitmask, PAGESIZE * 2);
		char *y = nanos6_lmalloc(PAGESIZE * blocks_per_node);

		// Check that nanos6_numa_alloc_block_interleave uses lmalloc
		// This makes an unsafe assumption on ordering of the allocated regions
		// assert(x < a && a < y);

		int node = 0;
		for(int block=0; block < blocks_per_node; block++) {
			#pragma oss task inout(a[PAGESIZE*block;PAGESIZE]) node(0) label("check")
			{
				// On which NUMA node is the current CPU?
				int cpu = sched_getcpu();
				int numa_node = numa_node_of_cpu(cpu);
				int expect = (block / 2) % 2;

				// Which NUMA node is the page mapped to?
				void *pages = &a[PAGESIZE*block];
				a[PAGESIZE*block] = 0; // fault it, otherwise will get -EFAULT
				int status;
				move_pages(/* pid */ 0, /* count */ 1, &pages, NULL, &status, MPOL_MF_MOVE);

				// Print out
				printf("Block %d (%p) expect NUMA node %d: CPU NUMA node: %d page NUMA node: ",
						block,
						&a[PAGESIZE*block],
						expect,
						numa_node);

				if (status >= 0) {
					printf("%d\n", status);
				} else {
					switch(status) {
						case -EACCES: printf("-EACCES\n"); break;
						case -EBUSY:  printf("-EBUSY\n"); break;
						case -EFAULT: printf("-EFAULT\n"); break;
						case -EIO:    printf("-EIO\n"); break;
						case -EINVAL: printf("-EINVAL\n"); break;
						case -ENOMEM: printf("-ENOMEM\n"); break;
						default:
							printf("%d\n", status);
							break;
					 }
				}

				assert_that(numa_node == expect);
				assert_that(status == expect);

				usleep(10000);
			}
		}
		#pragma oss taskwait

		nanos6_numa_free(a);
	}

    return 0;
}
