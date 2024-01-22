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



const size_t SIZE = 1024;

#pragma oss task out(arr[0;nr_elements]) label("init")
void init_range_float(float *arr, size_t nr_elements, float start)
{
	for (size_t i = 0; i < nr_elements; ++i)
		arr[i] = i + start;
}

#pragma oss task in(arr1[0;nr_elements]) \
	in(arr2[0;nr_elements]) out(sum[0;nr_elements]) node(nodeid) label("add")
void add(float *arr1, float *arr2, float *sum, size_t nr_elements, int nodeid)
{
	const int currentNode = nanos6_get_cluster_node_id();
	fail_if(currentNode != nodeid, "Wrong node %d != %d\n",  currentNode, nodeid);
	for (size_t i = 0; i < nr_elements; ++i) {
		sum[i] = arr1[i] + arr2[i];
	}
}


bool test_sum()
{
	const size_t SIZE_2 = SIZE * SIZE;

	float *array1 = array_alloc_float(SIZE_2, true);
	float *array2 = array_alloc_float(SIZE_2, true);
	float *sum = array_alloc_float(SIZE_2, true);

	init_range_float(array1, SIZE_2, 1.0);
	init_range_float(array2, SIZE_2, 1.0);
	init_range_float(sum, SIZE_2, 0);

	const size_t wsize = nanos6_get_num_cluster_nodes();

	for (size_t i = 0; i < SIZE_2 ; i+=SIZE) {
		int nodeid = i % wsize;
		add(&array1[i], &array2[i], &sum[i], SIZE, nodeid);
	}

	#pragma oss taskwait


	for (size_t i = 0; i < SIZE_2 ; i++) {
		const float val = 2.0 * i + 2.0;
		fail_if(sum[i] != val, "sum[i] = %f expected %f\n", sum[i], val);
	}

	array_free_float(array1, SIZE_2, true);
	array_free_float(array2, SIZE_2, true);
	array_free_float(sum, SIZE_2, true);

	return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(test_sum);

	return run_test(&test);
}
