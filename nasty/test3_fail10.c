/*
 * Copyright (C) 2021  Jimmy Aguilar Mena
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


bool weakout_notouch()
{
    int b = -1;  // local

	#pragma oss task out(b) node(1) label("task0")
    {
		b = -42;

		// This is the root of the problem.
		#pragma oss task weakout(b) node(2) label("task1")
		{
			printf("b = %d\n", b);
		}
	}

	#pragma oss task in(b) node(0) label("task2")
	{
		fail_if(b != -42, "Assertion failed, b = %d", b);
	}

	#pragma oss taskwait

    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(weakout_notouch);

	return run_test(&test);
}
