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

const size_t SIZE = 512;
const size_t TS = 8;

const size_t N = 4;

bool deserialize_and_check()
{
	bool success = false;

	int **arr = malloc(N * sizeof(int *));

	for (size_t i = 0; i < N; ++i) {
		arr[i] = array_alloc_int(SIZE, false);
	}

	// Deserialize the matrix (false at the end)
	for (size_t i = 0; i < N; ++i) {
		int ret = nanos6_serialize(arr[i], SIZE * sizeof(int), 3141594, i, false);
		assert_that(ret == 0);
	}

	for (size_t i = 0; i < N; ++i) {
		int *tmp = arr[i];

		const int nNodes = nanos6_get_num_cluster_nodes();
		int nodeId = i % nNodes;

		#pragma oss task in(tmp[0;SIZE]) inout(success) node(nodeId)
		{
			for (size_t j = 0; j < SIZE; ++j) {
				assert_that(tmp[j] == i * j);
			}
		}
	}

	#pragma oss taskwait

	for (size_t i = 0; i < N; ++i) {
		array_free_int(arr[i], SIZE, false);
	}

	free(arr);

	return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(deserialize_and_check);

	return run_test(&test);
}
