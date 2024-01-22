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

/*
 * test_memory_consumption.c:
 * Tests memory consumption in Nanos6 memory pool allocator.
 * 
 * Test program based on "bool_bug.c" from Omar Shaaban.
 *
 * This program checks for excessive memory consumption, which was fixed in the
 * following commits of the cluster_develop_dlb_paul branch of Nanos6@cluster:
 *
 *   21b0f686 Non-object memory allocators not in per-CPU pool 
 *   2946c172 CPUObjectCache returns surplus free objects
 */

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <nanos6/cluster.h>


bool check_memory_consumption()
{
    size_t N = 10*1024*1024;
    
    int *Q;
    
    int itr = 0;
    while(itr < 2048)
    {
        Q = (int *)nanos6_lmalloc(N * sizeof(int));
#if 1
        if ((itr & (itr-1)) == 0)
			printf("Iteration %d\n", itr, Q);
#endif
        assert_that(Q);

		// This no longer works with support for taskwait noflush
		// The lfree must be in the same task as the lmalloc
		// #pragma oss task node(0) label("task0")
		{
			nanos6_lfree(Q, N*sizeof(int));
		}
        // #pragma oss taskwait
        ++itr;
    }

    #pragma oss taskwait 
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(check_memory_consumption);

	return run_test(&test);
}
