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

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int ref = 123;

	#pragma oss task out(a[0;10]) node(1) label("my-task1")
	{
		for(int i=0; i<10; i++) {
			a[i] = ref;
		}
	}

	#pragma oss task for inout(a[0;10]) chunksize(2) label("my-taskfor1")
	for(int i = 0; i < 10; i++) {
		printf("taskfor1: a[%d] = %08x expect %08x\n", i, a[i], ref);
		assert_that(a[i] == ref);
		a[i] += 1;
    }

	#pragma oss task in(a[0;10]) node(1) label("my-task2")
	{
		for(int i=0; i<10; i++) {
			printf("task2: a[%d] = %08x expect %08x\n", i, a[i], ref+1);
			assert_that(a[i] == ref+1);
		}
	}

	#pragma oss taskwait

	nanos6_dfree(a, 1000 * sizeof(int));

    return(0);
}
