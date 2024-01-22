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
#include <nanos6.h>

void func3(int *a)
{
    sleep(1);
    printf("func3 on %d\n", nanos6_get_cluster_node_id());
    a[1] = 10;
    sleep(1);
}

void func6(int *a)
{
    a[0] = 1; // write on node 0
    sleep(1);
    printf("func6 on %d\n", nanos6_get_cluster_node_id());
    sleep(1);
}

void func2(int *a)
{
    sleep(1);
    printf("func2 on %d\n", nanos6_get_cluster_node_id());
    sleep(1);

	#pragma oss task inout(a[1;1]) node(0) label("func3")
    func3(a);

	#pragma oss taskwait // correctly fetches from node 0
    // assert_that(a[1] == 10);

	#pragma oss task inout(a[0;1]) node(0) label("func6")
    func6(a);
}

void func1(int *a)
{
	#pragma oss task inout(a[0;2]) node(1) label("func2")
    func2(a);

	#pragma oss task inout(a[0;1]) node(1) label("func7")
    {
        // no copy as thinks data already / still here
        sleep(1);
        printf("func7 on %d\n", nanos6_get_cluster_node_id());

        // should fetch from node 0
        assert_that(a[0] == 1);
    }
}

bool fail11()
{
    int *a = nanos6_lmalloc(1000 * sizeof(int));

    func1(a);

	#pragma oss taskwait
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail11);

	return run_test(&test);
}
