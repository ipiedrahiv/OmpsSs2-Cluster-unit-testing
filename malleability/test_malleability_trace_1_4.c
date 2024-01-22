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

int main()
{
	int num_elems = 100000;

	int *arr = nanos6_dmalloc(
		num_elems * sizeof(int), nanos6_equpart_distribution, 0, NULL
	);

	printf("Start first loop\n");
	#pragma oss taskloop label("first taskloop (1 node)") out(arr[j])
	for (int j=0; j<num_elems; j++) {
		usleep(50);
		arr[j] = 10;

		if (j == 0) {
			printf("IM1 am at zero: %d %d\n", arr[j], nanos6_get_cluster_node_id());
		}
	}
	printf("Done first loop\n");

 	nanos6_cluster_resize(3);

	printf("Start second loop\n");
	#pragma oss taskloop label("second taskloop (4 nodes)") inout(arr[j])
 	for (int j=0; j<num_elems; j++) {
 		usleep(500);
		//printf("arr[%d](%p) = %d\n", j, &arr[j], arr[j]);
 		assert_that(arr[j] == 10);
 		arr[j] = 20;

		if (j == 0) {
			printf("IM2 am at zero: %d %d\n", arr[j], nanos6_get_cluster_node_id());
		}
 	}
	printf("Done second loop\n");

	printf("IM3 am at zero: %d %d\n", arr[0], nanos6_get_cluster_node_id());

 	nanos6_cluster_resize(-3);

	printf("IM4 am at zero: %d %d\n", arr[0], nanos6_get_cluster_node_id());

	#pragma oss taskloop label("third taskloop (1 node)") inout(arr[j])
 	for (int j=0; j<num_elems; j++) {
 		usleep(500);
 		assert_that(arr[j] == 20);
 		arr[j] = 30;
 	}

 	nanos6_cluster_resize(3);

	#pragma oss taskloop label("fourth taskloop (4 nodes)") inout(arr[j])
 	for (int j=0; j<num_elems; j++) {
 		usleep(500);
 		assert_that(arr[j] == 30);
 		arr[j]=40;
 	}

	#pragma oss taskwait
	nanos6_dfree(arr, num_elems * sizeof(int));
	printf("Done\n");

	return 0;
}
