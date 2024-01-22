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

// This is a sort of "hack" to assert the removed processes are effectively
// removed and not hang at the MPI_Finalize.
void destructor() __attribute__((destructor));
void destructor()
{
    printf ("Destructing\n");
}

int main()
{
	fail_if(nanos6_get_num_cluster_nodes() != 1,
	        "Wrong cluster size before 1 spawn.");

	nanos6_cluster_resize(2);

	fail_if(nanos6_get_num_cluster_nodes() != 3,
	        "Wrong cluster size after 1 spawn(2) %d != %d\n.",
	        nanos6_get_num_cluster_nodes(), 3);

	nanos6_cluster_resize(-2);
	printf("After first shrink\n");

	fail_if(nanos6_get_num_cluster_nodes() != 1,
	        "Wrong cluster size after first shrink(1) %d != %d\n.",
	        nanos6_get_num_cluster_nodes(), 1);

	sleep(3);
	printf("Wakeup\n");

	return 0;
}
