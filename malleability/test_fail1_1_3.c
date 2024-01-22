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

#include "util.h"

#include <stdlib.h>
#include <unistd.h>

int main()
{
	int actual_nodes = 1;

	int *arr = nanos6_lmalloc(sizeof(int));
	int expected = 123;
	arr[0] = expected;

	srand(3);

	int nodes;

	nodes = 2;
	printf("A Will update to %d nodes\n", nodes);
	nanos6_cluster_resize(1);
	actual_nodes++;

	for(int node=0; node < nodes; node++) {
		#pragma oss task node(node) inout(arr[0])
		{
			fail_if(nanos6_get_cluster_node_id() != node,
			        "Task on wrong cluster node.");
			fail_if(arr[0] != expected,
			        "Incorrect data value passed to task");
			arr[0] += 100;
			printf("node %d is ", node);
			system("hostname");
		}
		expected += 100;
	}

	nodes = 1;
	printf("B Will update to %d nodes\n", nodes);
	// Migrate data by hand
	#pragma oss task node(0) inout(arr[0])
	{
		sleep(1);
		printf("Migration: got %d expected %d\n", arr[0], expected);
		fail_if(arr[0] != expected, "Incorrect value on migration");
	}

	// Resize
	nanos6_cluster_resize(-1);
	actual_nodes--;

	for(int node=0; node < nodes; node++) {
		#pragma oss task node(node) inout(arr[0])
		{
			fail_if(nanos6_get_cluster_node_id() != node,
			        "Task on wrong cluster node.");
			fail_if(arr[0] != expected,
			        "Incorrect data value passed to task");
			arr[0] += 100;
			printf("node %d is ", node);
			system("hostname");
		}
		expected += 100;
	}

	sleep(2);

	nodes = 2;
	printf("C Will update to %d nodes\n", nodes);
	if (nodes < actual_nodes) {
		// Migrate data by hand
		#pragma oss task node(0) inout(arr[0])
		{
			sleep(1);
			printf("Migration: got %d expected %d\n", arr[0], expected);
			fail_if(arr[0] != expected, "Incorrect value on migration");
		}
	}

	// Resize
	while (nodes > actual_nodes) {
		printf("Spawning 1");
		nanos6_cluster_resize(1);
		actual_nodes++;
	}
	while (nodes < actual_nodes) {
		printf("Shrinking 1");
		nanos6_cluster_resize(-1);
		actual_nodes--;
	}

	for(int node=0; node < nodes; node++) {
		#pragma oss task node(node) inout(arr[0])
		{
			fail_if(nanos6_get_cluster_node_id() != node,
			        "Task on wrong cluster node.");
			fail_if(arr[0] != expected,
			        "Incorrect data value passed to task");
			arr[0] += 100;
			printf("node %d is ", node);
		}
		expected += 100;
	}

	nodes = 1;
	printf("D Will update to %d nodes\n", nodes);
	if (nodes < actual_nodes) {
		// Migrate data by hand
		#pragma oss task node(0) inout(arr[0])
		{
			sleep(1);
			printf("Migration: got %d expected %d\n", arr[0], expected);
			fail_if(arr[0] != expected, "Incorrect value on migration");
		}
	}

	#pragma oss taskwait
	nanos6_lfree(arr, sizeof(int));

	return 0;
}
