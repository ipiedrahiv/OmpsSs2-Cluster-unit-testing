/*
 * Copyright (C) 2022  Paul Carpenter
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


#define MIN(a,b) ((a)>(b)?(b):(a))

const int N = 50;

int main(void)
{
    int *array = nanos6_lmalloc(N * sizeof(int));
	for (int i = 0; i < N; i++) {
		#pragma oss task out(array[i]) node(i%3) label("init")
		{
			if (i==0) {
				array[i] = 0;
			} else {
				array[i] = -1;
			}
		}
	}

	// Taskloop distributed over all nodes with iteration-to-iteration dependence
	int nodes = nanos6_get_num_cluster_nodes();
	int itersPerNode = (N-1+nodes-1)/nodes;
	#pragma oss taskloop in(array[i]) inout(array[i+1]) grainsize(1) label("taskloop")
	for (int i = 0; i < N-1; ++i) {
		// printf("A %d on %d (%d;%d)\n", i, nanos6_get_cluster_node_id(), array[i], array[i+1]);
		assert_that(array[i] == i);
		assert_that(array[i+1] == -1);
		array[i+1] = i+1;
		assert_that(nanos6_get_cluster_node_id() == (i/itersPerNode)); // assume blocked scheduling
	}

	#pragma oss taskwait

	for (int i = 0; i < N; ++i) {
		// printf("array[%d] = %d\n", i, array[i]);
		assert_that(array[i] == i);
	}

	printf("Test passed\n");
}

	
