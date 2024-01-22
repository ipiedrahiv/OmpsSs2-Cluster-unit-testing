/*
 * Copyright (C) 2022  Paul Capenter
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
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "nanos6.h"


int main(int argc, char **argv)
{
    int *a = nanos6_lmalloc(10*sizeof(int));
	a[0] = 1;
	a[5] = 101;

	#pragma oss task weakinout(a[0;10]) node(1) label("weak1")
	{
		sleep(1);
		#pragma oss task inout(a[0;10]) node(1) label("strong1")
		{
			printf("\n\nStart task A on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 1);
			assert_that(a[5] == 101);
			sleep(3);
			a[0] = 2;
			a[5] = 102;
			printf("\n\nDone task A on %d\n", nanos6_get_cluster_node_id());
		}
	}
	#pragma oss task weakinout(a[0;5]) node(1) label("weak2")
	{
		sleep(1);
		#pragma oss task inout(a[0;5]) node(1) label("strong2")
		{
			printf("\n\nStart task B on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[0] == 2);
			sleep(3);
			a[0] = 3;
			printf("\n\nDone task B on %d\n", nanos6_get_cluster_node_id());
		}
	}
	#pragma oss task weakinout(a[5;10]) node(1) label("weak3")
	{
		sleep(1);
		#pragma oss task inout(a[5;10]) node(1) label("strong3")
		{
			printf("\n\nStart task C on %d\n\n", nanos6_get_cluster_node_id());
			assert_that(a[5] == 102);
			sleep(3);
			a[5] = 103;
			printf("\n\nDone task C on %d\n", nanos6_get_cluster_node_id());
		}
	}

	#pragma oss taskwait
	assert_that(a[0] == 3);
	assert_that(a[5] == 103);

    return 0;
}
