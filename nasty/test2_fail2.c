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

/*
 * test_fail2.c:
 * This test program is based on "MicroPP".
 *
 * This program was failing on two nodes of MareNostrum 4.
 */

#include "util.h"
#include <stdio.h>
#include "nanos6.h"

int main(int argc, char **argv)
{
    int n = 1000;
    int *a = nanos6_dmalloc(4 * n * sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(2 * sizeof(int));

    for (int k = 2 * n; k < 3 * n; ++k) {
        #pragma oss task out(a[k]) in(b[0;2]) node(0) label("task0")
        {
            // Run on node 0
            // printf("write %p a[%d] on %d\n", &a[k], k, nanos6_get_cluster_node_id());
            // sleep(1);
            a[k] = 1;
        }
    }

    for (int k = 2 * n; k < 3 * n; ++k) {
        #pragma oss task in(a[k]) out(a[3*n;n]) node(1) label("task1")
        {
            // Run on node 1
            // printf("read %p a[%d] on %d\n", &a[k], k, nanos6_get_cluster_node_id());
            if (a[k] != 1) {
                printf("Error in check %p a[%d]: %d != 1\n", &a[k], k, a[k]);
            }
            assert_that(a[k] == 1);
        }
    }

	#pragma oss taskwait

    return 0;
}
