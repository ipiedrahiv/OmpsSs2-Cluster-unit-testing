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
#include <unistd.h>


int main(int argc, char **argv)
{
	int nodes = nanos6_get_num_cluster_nodes();
	char *ALL_MEMORY = NULL;
	int N = 10;
	int M = 10;
	int **A = nanos6_lmalloc(N*sizeof(int*));
	int *B = nanos6_lmalloc(N*M*sizeof(int));
	printf("A: %p B: %p\n", A, B);

	for (int k = 0; k < N; ++k) {
		#pragma oss task concurrent(A[k]) weakconcurrent(ALL_MEMORY[1;-2]) node(k%nodes) label("T1") wait
		{
			printf("Start %d on %d\n", k, nanos6_get_cluster_node_id());
			A[k] = nanos6_lmalloc(M * sizeof(int));
			printf("Task %d: %p %p\n", k, &A[k], A[k]);
			#pragma oss task inout(A[k][0]) in(A[k])
			{
				A[k][0] = 100+k;
			}
			sleep(1);
			printf("End %d on %d\n", k, nanos6_get_cluster_node_id());
		}
	}
	#pragma oss taskwait

	for (int k = 0; k < N; ++k) {
		// #pragma oss task in(A[k]) weakin(ALL_MEMORY[1;-2]) node(1) label("T2")
		#pragma oss task in(A[k]) weakin(ALL_MEMORY[1;-2]) node(1) label("T2")
		{
			#pragma oss task in(A[k]) in(A[k][0;10]) node(nanos6_cluster_no_offload) label("T2a")
			{
				printf("Test A[%d][0] (%p) on %d: get %d should be on node %d\n", k, &A[k][0], nanos6_get_cluster_node_id(), A[k][0], k%nodes);
				assert_that(A[k][0] == 100+k);
			}
		}
	}

	#pragma oss taskwait
	printf("Test passed\n");
	return 0;
}
