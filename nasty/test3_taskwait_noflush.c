/*
 *  This source code is part of Micropp: a Finite Element library
 *  to solve composite materials micro-scale problems.
 *
 *  Copyright (C) - 2018
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "util.h"
#include <stdio.h>
#include "nanos6.h"


bool taskwait_noflush()
{
	// Allocate memory
	char *a = (char *)nanos6_lmalloc(1024 * sizeof(int));
	a[0] = 10;


	printf("hello\n");
	#pragma oss task out(a[0;200]) node(1) label("task0")
	{
		a[0] = 20;
		sleep(1);
		printf("Task done!\n");
	}

	#pragma oss taskwait noflush
	// #pragma oss taskwait
	printf("a[0] = %d\n", a[0]);

	switch(a[0])
	{
		case 10:
			printf("PASS: Did not flush\n");
			break;
		case 20:
			printf("FAIL: Did flush\n");
			break;
		default:
			assert_that(0);
	}

	#pragma oss task in(a[0;200]) node(1) label("task1")
	{
		assert_that(a[0] == 20);
		printf("Task done!\n");
	}

    return true;
}
	
int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(taskwait_noflush);

	return run_test(&test);
}
