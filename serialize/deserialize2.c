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

bool deserialize_and_check()
{
	bool success = false;
	int *arr1 = array_alloc_int(SIZE, false);
	int *arr2 = array_alloc_int(SIZE, false);

	// Deserialize the matrix (false at the end)
	if (nanos6_serialize(arr1, SIZE * sizeof(int), 3141593, 1, false) != 0) {
		return false;
	}

	if (nanos6_serialize(arr2, SIZE * sizeof(int), 3141593, 2, false) != 0) {
		return false;
	}

	#pragma oss task in(arr1[0;SIZE]) inout(success)
	{
		success = true;
		for (size_t i = 0; i < SIZE; ++i) {
			int val = i / TS;
			if (arr1[i] != val) {
				success = false;
				printf("arr1[%zu] = %d expected %d\n", i, arr1[i], val);
				break;
			}
		}
	}

	#pragma oss task in(arr2[0;SIZE]) inout(success)
	{
		if (success) {
			for (size_t i = 0; i < SIZE; ++i) {
				if (arr2[i] != i) {
					success = false;
					printf("arr2[%zu] = %d expected %d\n", i, arr2[i], i);
					break;
				}
			}
		} else {
			printf("First check failed, not executed second\n");
		}
	}

	#pragma oss taskwait

	array_free_int(arr1, SIZE, false);
	array_free_int(arr2, SIZE, false);

	return success;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(deserialize_and_check);

	return run_test(&test);
}
