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

int main()
{
	#pragma oss task
	{
		fail_if(nanos6_get_cluster_node_id() != 0,
		        "Task in wrong cluster node.");
	}

	char name[256];
	gethostname(name, 256);
	printf("NAME %s\n", name);


	nanos6_cluster_resize(1);

	#pragma oss task node(1) label("offloaded")
	{
		char name[256];
		gethostname(name, 256);
		printf("NAME %s\n", name);

		fail_if(nanos6_get_cluster_node_id() != 1,
		        "Task in wrong cluster node.");
	}

	#pragma oss taskwait

	return 0;
}
