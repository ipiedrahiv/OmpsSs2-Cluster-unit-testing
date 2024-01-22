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


#pragma oss task out(b[1;1]) node(1) label("func0")
void func0(int *b)
{
    printf("func0 on %d\n", nanos6_get_cluster_node_id());
    b[1] = 1;
    sleep(1);
}

#pragma oss task out(b[1;1]) node(1) label("func1")
void func1(int *b)
{
    printf("func1 on %d\n", nanos6_get_cluster_node_id());
    b[1] = 2;
}

#pragma oss task inout(b[0;1]) inout(b[1;1]) node(0) label("func4")
void func4(int *b)
{
    printf("func4 on %d\n", nanos6_get_cluster_node_id());
    printf("b[0] at %p, b[1] at %p\n", b, b+1);
    printf("Get values %x and %x\n", b[0], b[1]);
    //assert_that(b[0] == 1); // nobody initialized this
    fail_if(b[1] != 2, "Assertion failed\n");   // fails about 1 in 5 times with random scheduler
}

void func3(int *b)
{
    printf("func3 on %d\n", nanos6_get_cluster_node_id());
    func0(b);

	#pragma oss taskwait

    func1(b);

    // sleep(1);  // makes no difference
}


bool fail6()
{
    int *b = array_alloc_int(1000, false);

	#pragma oss taskwait

	#pragma oss task weakinout(b[0;1]) weakinout(b[1;1]) label("func3") node(1)
    func3(b);

    func4(b);

	#pragma oss taskwait
	array_free_int(b, 1000, false);

    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail6);

	return run_test(&test);
}
