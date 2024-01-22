/*
 * Copyright (C) 2020 Paul Carpenter and Omar Shaaban
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



void func0(int *b)
{
    printf("func0 on %d\n", nanos6_get_cluster_node_id());
    #pragma oss task inout(b[0;1]) node(1) label("task0")
    {
        printf("func1 on %d\n", nanos6_get_cluster_node_id());
        b[0] = 2;
    }

    #pragma oss task inout(b[0;1]) node(0) label("task1")
    {
        printf("func2 on %d\n", nanos6_get_cluster_node_id());
        printf("b[0] is %d, should be %d\n", b[0], 2);
        assert_that(b[0] == 2);
    }
}


bool fail8()
{
    int *b = nanos6_lmalloc(1000 * sizeof(int));

    b[0] = 1;

    #pragma oss task inout(b[0;1]) node(1) label("func0")
    func0(b);

    #pragma oss taskwait
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail8);

	return run_test(&test);
}
