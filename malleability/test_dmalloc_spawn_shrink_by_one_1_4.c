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



int main()
{
	int num_elems = 16;
	int actual_nodes = 1;
	const int max_nodes = 4;

	int *arr = nanos6_dmalloc(num_elems * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	printf("arr = %p\n", arr);
	int expected;

	srand(3);

	int N = 30;
	for (int i=0; i<N; i++) {
		int nodes = (rand() % max_nodes) + 1;
		printf("%d. Will update to %d nodes\n", i, nodes);

#if 1
		// Remove this once data migration is working
		if (nodes < actual_nodes) {
			// Migrate data by hand to one of the remaining nodes
			int target_node = rand() % nodes;
			#pragma oss task node(target_node) inout(arr[0;num_elems])
			{
				printf("Migration: got %d expected %d\n", arr[0], expected);
				for(int j=0; j<num_elems; j++) {
					fail_if(arr[j] != expected+j, "Incorrect value on migration");
				}
			}
		}
#endif

		// Resize
		while (nodes > actual_nodes) {
			nanos6_cluster_resize(1);
			actual_nodes++;
		}
		while (nodes < actual_nodes) {
			nanos6_cluster_resize(-1);
			actual_nodes--;
		}

		fail_if(nanos6_get_cluster_node_id() != 0,
		        "Main on wrong cluster node.");
		fail_if(nanos6_get_num_cluster_nodes() != nodes,
		        "Wrong number of cluster nodes.");

		for(int node=0; node < nodes; node++) {
			#pragma oss task node(node) inout(arr[0;num_elems])
			{
				fail_if(nanos6_get_cluster_node_id() != node,
						"Task on wrong cluster node.");
				if (i > 0) {
					for(int j=0; j<num_elems; j++) {
						fail_if(arr[j] != expected +j,
							"Incorrect data value passed to task");
						arr[j] += 100;
					}
				} else {
					for(int j=0; j<num_elems; j++) {
						arr[j] = expected+j+100;
					}
				}
				// printf("node %d increases to %d, and is ", node, arr[0]);
				// system("hostname");
			}
			expected += 100;
		}
	}

	#pragma oss taskwait
	nanos6_dfree(arr, num_elems * sizeof(int));

	return 0;
}
