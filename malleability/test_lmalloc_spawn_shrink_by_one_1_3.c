/*
 * Copyright (C) 2022  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include "util.h"

#include <unistd.h>
#include <limits.h>

int main()
{
	int actual_nodes = 1;
	int nodes = actual_nodes;
	int last;

	int *arr = nanos6_lmalloc(sizeof(int));
	int expected = 123;
	arr[0] = expected;
	printf("&arr[0] = %p\n", arr);

	srand(3);

	int N = 30;
	for (int i = 0; i < N; i++) {
		int last = nodes;
		int nodes = (rand() % 3) + 1;
		printf("%d. Will update %d to %d nodes\n", i, last, nodes);
		if (nodes < actual_nodes) {
			// Migrate data by hand
			#pragma oss task node(0) inout(arr[0])
			{
				sleep(1);
				printf("%d. Migration: got %d expected %d\n", i, arr[0], expected);
				fail_if(arr[0] != expected, "Incorrect value on migration\n");
			}
		}

		#pragma oss taskwait

		// Resize
		while (nodes > actual_nodes) {
			nanos6_cluster_resize(1);
			printf("%d. Spawning 1\n", i);
			actual_nodes++;
		}
		while (nodes < actual_nodes) {
			nanos6_cluster_resize(-1);
			printf("%d. Shrinking 1\n", i);
			actual_nodes--;
		}

		fail_if(nanos6_get_cluster_node_id() != 0,
		        "Main on wrong cluster node.\n");
		fail_if(nanos6_get_num_cluster_nodes() != nodes,
		        "Wrong number of cluster nodes.\n");

		for(int node = 0; node < nodes; node++) {
			#pragma oss task node(node) inout(arr[0])
			{
				char hostname[HOST_NAME_MAX];

				fail_if(gethostname(hostname, HOST_NAME_MAX) != 0,
						"Couldn't get hostname.");

				fail_if(nanos6_get_cluster_node_id() != node,
						"Task on wrong cluster node.\n");
				fail_if(arr[0] != expected,
						"Incorrect data value passed to task\n");

				arr[0] += 100;
				printf("%d. node %d(%s) increases to %d\n",
				       i, node, hostname, arr[0]);
			}
			expected += 100;
		}
	}

	#pragma oss taskwait
	nanos6_lfree(arr, sizeof(int));

	return 0;
}
