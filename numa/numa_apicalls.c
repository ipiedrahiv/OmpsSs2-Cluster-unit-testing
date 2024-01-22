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

	nanos6_bitmask_t bitmask;
	nanos6_bitmask_set_wildcard(&bitmask, NUMA_ANY_ACTIVE);
	srand(1);

	// The NUMA API internally uses lmalloc when built for clusters. It assumes that
	// the allocated region is on a 4 KB boundary. We check that this is true.
	int size = 1;
	while (size < 1000000) {
		int blockSize = rand() % (size * 3);
		char *a = (char *)nanos6_numa_alloc_block_interleave(size, &bitmask, blockSize);
		printf("size=%7d bs=%7d %p\n", size, blockSize, a);

		if (size >= PAGESIZE) {
			assert_that( ((size_t)a & (PAGESIZE-1)) == 0);
		}
		size = (1+size * 3) / 2;
	}

    return 0;
}
