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

bool init_and_serialize()
{
	bool success = false;

	int **arr = malloc(N * sizeof(int *));

	for (size_t i = 0; i < N; ++i) {
		arr[i] = array_alloc_int(SIZE, false);
	}

	for (size_t i = 0; i < N; ++i) {
		int *tmp = arr[i];

		for (size_t j = 0; j < SIZE; j += TS) {

			#pragma oss task out(tmp[j;TS])
			{
				for (size_t k = j; k < j + TS; ++k) {
					tmp[k] = i * k;
				}
			}
		}
	}

	// Serialize the matrix (true at the end)
	success = true;
	for (size_t i = 0; i < N; ++i) {
		int ret = nanos6_serialize(arr[i], SIZE * sizeof(int), 3141594, i, true);
		assert_that(ret == 0);
	}

	#pragma oss taskwait

	for (size_t i = 0; i < N; ++i) {
		array_free_int(arr[i], SIZE, false);
	}

	free(arr);

	return success;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(init_and_serialize);

	return run_test(&test);
}
