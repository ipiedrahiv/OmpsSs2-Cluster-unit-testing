/*
 * Copyright (C) 2022  Jimmy Aguilar Mena
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


bool test_lmalloc()
{

	#pragma oss task label("A") node(1)
	{
		int *x = nanos6_lmalloc(10 * sizeof(int));
		#pragma oss task label("B") out(x[0;10]) node(0)
		{
			x[0] = 1;
		}
		#pragma oss taskwait
		assert_that(x[0] == 1);
		nanos6_lfree(x, 10*sizeof(int));
	}

	#pragma oss taskwait

	return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(test_lmalloc);

	return run_test(&test);
}
