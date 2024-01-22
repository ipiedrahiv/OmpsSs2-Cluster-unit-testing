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

// Example adapted from https://pm.bsc.es/ftp/ompss-2/doc/spec/directives/index.html#task-for-clause

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>

#include <util.h>

int main(void)
{
    int *array = nanos6_lmalloc(ARRAY_SZ * sizeof(int));
	for (int i = 0; i < ARRAY_SZ; i += TASK_SZ) {
		#pragma oss task out(array[i;TASK_SZ]) label("task")
		for (int j = i; j < i + TASK_SZ; ++j) {
			array[j] = 20;
		}
	}

	#pragma oss task for inout(array[0;ARRAY_SZ]) chunksize(TASK_SZ) label("taskfor") node(1)
	for (int i = 0; i < ARRAY_SZ; ++i) {
		printf("Taskfor %d\n", i);
		array[i] += 100;
		assert_that(nanos6_get_cluster_node_id() == 1);
	}

	#pragma oss taskwait

	for (int i = 0; i < ARRAY_SZ; ++i) {
		assert_that(array[i] == 120);
	}

	printf("Test passed\n");

	return 0;
}

