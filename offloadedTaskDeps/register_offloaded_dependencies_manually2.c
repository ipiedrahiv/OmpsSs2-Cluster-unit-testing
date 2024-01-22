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
#include "util.h"

struct foo{
	int *a;
	int n;
};

int main(int argc, char *argv[]) 
{
	struct foo bar;
	bar.n = 100;
	bar.a = (int *)nanos6_lmalloc(bar.n * sizeof(int));
	bar.a[0] = 456;

	#pragma oss task in(bar) in(bar.a[0;100]) node(1) label("B")
	{
		assert_that(bar.a[0] == 456);
		printf("bar.n  = %d \n", bar.n);
	}

	#pragma oss taskwait
	printf("Test Passed.\n");
	return 0;
}
