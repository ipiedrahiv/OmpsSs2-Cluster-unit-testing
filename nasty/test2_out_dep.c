/*
 * Copyright (C) 2020 Paul Carpenter
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

/*
 * test_out_dep.c:
 * Tests location tracking for "out" dependencies on offloaded tasks.
 * 
 * This program checks that the location of an out dependency is correctly
 * set. This test was previously known as "fail5.c". It was fixed in the 
 * following commit of the cluster_develop_dlb_paul branch of Nanos6@cluster:
 *
 *   db638cb1 Fix cluster location tracking for out dependencies
 */
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


// Fails if func0 and func1 both execute on node 1

void func1(int *a)
{
    printf("func3 on %d\n", nanos6_get_cluster_node_id());
    assert_that(a[0] == 1);
}


int main(void)
{
    int *a = nanos6_lmalloc(4 * sizeof(int));
    int *b = nanos6_dmalloc(16 * sizeof(int), nanos6_equpart_distribution, 0, NULL);

#pragma oss task out(a[0;1]) inout(b[8;8]) node(1) label("task0")
    {
        printf("func0 on %d\n", nanos6_get_cluster_node_id());
        a[0] = 1;

        #pragma oss task in(a[0;1]) inout(b[8;8]) node(1) label("func1")
        func1(a);
    }
    
    #pragma oss taskwait
    printf("Test passed [./nasty.py --tasks 200 --no-hash]\n");
    return(0);
}
    
