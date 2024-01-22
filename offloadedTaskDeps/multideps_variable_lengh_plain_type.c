/*
 * Copyright (C) 2022  Omar Shaaban
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

#include <nanos6.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main(int argx, char **argv) {

	int m = 5;
	//	each a[i] will have length n[i] 
	int n[5];
	int *a[m];

	for (size_t i = 0; i < m; ++i)
		n[i] = i*2 + 2;

	for (size_t i = 0; i < m; ++i){
		a[i] = (int *)nanos6_lmalloc(n[i] * sizeof(int));
		for (size_t j = 0; j < n[i]; ++j)
			a[i][j] = i + 100 + j;
	}

	#pragma oss task in({a[i], i=0;m}) inout({a[i][0;n[i]], i=0;m}) node(1) label("A")
	{
		for (size_t i = 0; i < m; ++i)
		{
			for (size_t j = 0; j < n[i]; ++j)
			{
				assert_that(a[i][j] == (i + 100 + j));
			}
		}
	}

	#pragma oss taskwait
	printf("Test Passed.\n");
	return 0;
}

