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

bool init_and_serialize()
{
	int *arr1 = array_alloc_int(SIZE, false);
	int *arr2 = array_alloc_int(SIZE, false);

	for (size_t i = 0; i < SIZE; i += TS) {
		int val = i / TS;

		#pragma oss task out(arr1[i;TS])
		{
			for (size_t j = i; j < i + TS; ++j) {
				arr1[j] = val;
			}
		}
	}

	for (size_t i = 0; i < SIZE; i += TS) {

		#pragma oss task out(arr2[i;TS])
		{
			for (size_t j = i; j < i + TS; ++j) {
				arr2[j] = j;
			}
		}
	}

	#pragma oss taskwait

	// Serialize the matrix (true at the end)
	if (nanos6_serialize(arr1, SIZE * sizeof(int), 3141593, 1, 1) != 0) {
		return false;
	}

	if (nanos6_serialize(arr2, SIZE * sizeof(int), 3141593, 2, 1) != 0) {
		return false;
	}

	#pragma oss taskwait
	printf("After taskwait3\n");

	array_free_int(arr1, SIZE, false);
	array_free_int(arr2, SIZE, false);

	return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(init_and_serialize);

	return run_test(&test);
}
