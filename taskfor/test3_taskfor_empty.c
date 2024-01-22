/*
 * Copyright (C) 2022  Paul Capenter
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
#include <unistd.h>
#include <string.h>
#include <nanos6.h>

#define MIN(a,b) ((a)>(b)?(b):(a))

const int N = 10;


int main(void)
{
    int *array = nanos6_lmalloc(N * sizeof(int));

	#pragma oss task for out(array[0;0]) chunksize(1024)  label("taskfor")
	for (int i = 0; i < 0; ++i) {
		printf("Taskfor %d\n", i);
		array[i] += 100;
	}

	sleep(1);
	printf("Before taskwait\n");
	#pragma oss taskwait

	printf("Test passed\n");
}
