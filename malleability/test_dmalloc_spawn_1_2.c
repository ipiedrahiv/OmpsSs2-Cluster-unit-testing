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

int main()
{
	int *arr = nanos6_dmalloc(
		ARRAY_SZ * sizeof(int), nanos6_equpart_distribution, 0, NULL);

	#pragma oss task out(arr[0;ARRAY_SZ]) node(0)
	{
		fail_if(nanos6_get_cluster_node_id() != 0,
		        "Task in wrong cluster node.");

		for (size_t i = 0; i < ARRAY_SZ; ++i) {
			arr[i] = i;
		}
	}

	nanos6_cluster_resize(1);

	#pragma oss task in(arr[0;ARRAY_SZ]) node(1)
	{
		fail_if(nanos6_get_cluster_node_id() != 1,
		        "Task in wrong cluster node %d != 1.", nanos6_get_cluster_node_id());

		for (size_t i = 0; i < ARRAY_SZ; ++i) {
			fail_if(arr[i] != i, "Comparison error index %d != %d\n", i, arr[i]);
		}
	}

	#pragma oss taskwait

	return 0;
}
