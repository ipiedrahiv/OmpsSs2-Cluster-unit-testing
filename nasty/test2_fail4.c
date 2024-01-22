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


bool fail4()
{
    int n = 50;
    int *a = nanos6_dmalloc(n*4 *sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(2*sizeof(int));
    int k;

    for(k = 2*n; k<3*n; k++)
    {
        #pragma oss task in(b[0;2]) label("task0")
        {
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail4);

	return run_test(&test);
}
