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

int main(int argc, char *argv[]) 
{
	int *a[10];
    int n = 10;

    a[0] = (int *)nanos6_lmalloc(n * sizeof(int));
    
	for (size_t i = 0; i < n; ++i)
        a[0][i] = i + 100;

    #pragma oss task in(a[0])inout(a[0][0; n]) node(1) label("A")
    {
        for (size_t i = 0; i < n; ++i)
            assert_that(a[0][i] == i + 100);
    }

    #pragma oss taskwait
	printf("Test Passed.\n");
    return 0;
}
