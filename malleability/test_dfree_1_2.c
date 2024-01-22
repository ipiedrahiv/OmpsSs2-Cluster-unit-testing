#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>

int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);

	assert(nanos6_get_num_cluster_nodes() == 1);
	nanos6_cluster_resize(1);

	nanos6_dfree(a, 1000 * sizeof(int));

    return(0);
}
