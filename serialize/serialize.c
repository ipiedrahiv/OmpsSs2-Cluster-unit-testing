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
	bool success = false;
	int *arr = array_alloc_int(SIZE, false);

	for (size_t i = 0; i < SIZE; i += TS) {
		int val = i / TS;

		#pragma oss task out(arr[i;TS])
		{
			for (size_t j = i; j < i + TS; ++j) {
				arr[j] = val;
			}
		}
	}

	#pragma oss taskwait
	// Serialize the matrix (true at the end)
	if (nanos6_serialize(arr, SIZE * sizeof(int), 3141592, 1, 1) == 0) {
		success = true;
	}

	#pragma oss taskwait

	array_free_int(arr, SIZE, false);
	return success;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(init_and_serialize);

	return run_test(&test);
}
