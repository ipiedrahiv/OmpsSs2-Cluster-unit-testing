/*
 * Copyright (C) 2020  Jimmy Aguilar Mena
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


const int NR_TASKS = 2;

#pragma oss task in(arr1[0]) in(arr2[0]) inout(sum[0]) label("task0")
void task_sum(int *arr1, int *arr2, int *sum)
{
	*sum = *arr1 + *arr2;
}


#pragma oss task weakin(arr1[0;N]) weakin(arr2[0;N]) weakinout(sum[0;N]) label("task1")
void nested_sum_task(int N, int *arr1, int *arr2, int *sum)
{
	if (N == 1) {
		task_sum(arr1, arr2, sum);
		return;
	}

	int split = N / 2;
	nested_sum_task(split, arr1, arr2, sum);
	nested_sum_task(split, &arr1[split], &arr2[split], &sum[split]);
}


#pragma oss task in(arr1[0;N]) out(success[0]) label("task2")
void test_sum(int N, int *arr1, int value, bool *success)
{
	*success = true;
	for (int i = 0; i < N; ++i) {
		if (arr1[i] != value) {
			*success = false;
			return;
		}
	}
}


const size_t SIZE = 128;

bool nested_sum()
{
	int *arr1 = array_alloc_int(SIZE, false);
	int *arr2 = array_alloc_int(SIZE, false);
	int *sum  = array_alloc_int(SIZE, false);
	bool success = false;

	#pragma oss task out(arr1[0;SIZE]) label("task3")
	init_buff_value_int(arr1, SIZE, 3);

	#pragma oss task out(arr2[0;SIZE]) label("task4")
	init_buff_value_int(arr2, SIZE, 4);

	#pragma oss task out(sum[0;SIZE]) label("task5")
	init_buff_value_int(sum, SIZE, 0);

	nested_sum_task(128, arr1, arr2, sum);

	#pragma oss taskwait

	test_sum(128, sum, 7, &success);

	#pragma oss taskwait

	array_free_int(arr1, SIZE, false);
	array_free_int(arr2, SIZE, false);
	array_free_int(sum, SIZE, false);

	return success;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(nested_sum);

	return run_test(&test);
}
