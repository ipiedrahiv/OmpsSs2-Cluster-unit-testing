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

struct foo {
	int *a;
	int length_a;
};

int main(int argx, char **argv) {
	int m = 5;
	int n = 10;
	struct foo bar[m];

	for (size_t i = 0; i < m; ++i) {
		bar[i].length_a = n;
		bar[i].a = (int *)nanos6_lmalloc(bar[i].length_a * sizeof(int));
		for (size_t j = 0; j < n; ++j)
			bar[i].a[j] = i + 100 + j;
	}

	#pragma oss task       								\
	in({bar[i], i=0;m})                            \
	inout({bar[i].a[0; bar[i].length_a], i = 0; m }) 	\
	node(1) label("A")
	{
		for (size_t i = 0; i < m; ++i) {
			for (size_t j = 0; j < bar[i].length_a; ++j) {
				assert_that(bar[i].a[j] == (i + 100 + j));
			}
		}
	}

	#pragma oss taskwait
	printf("Test Passed.\n");
	return 0;
}

