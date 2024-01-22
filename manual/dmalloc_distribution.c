#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <nanos6.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"

int main(void)
{
	int num_nodes = nanos6_get_num_cluster_nodes();
	int tasks_per_node = 10;
	int num_tasks = num_nodes * tasks_per_node;
	int *a = nanos6_dmalloc(num_tasks * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	char sentinel;

	for(int i=0; i < num_tasks; i++) {
		#pragma oss task inout(a[i]) inout(sentinel) node(nanos6_cluster_home) label("check")
		{
			int actual_node = nanos6_get_cluster_node_id();
			int expected_node = i / tasks_per_node;
			printf("Task %d on node %d (expect %d)\n", i, actual_node, expected_node);
			assert_that(actual_node == expected_node);
		}
	}

    #pragma oss taskwait

    return(0);
}

