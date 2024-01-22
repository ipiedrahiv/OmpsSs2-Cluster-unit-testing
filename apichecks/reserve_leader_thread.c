
/*
 * Copyright (C) 2020  Jimmy Aguilar Mena
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


int main(int argc, char *argv[])
{
	nanos6_cluster_info_t info;

	nanos6_get_cluster_info(&info);

	if (info.cluster_num_nodes == 1 && info.malleability_enabled == 0) {
		// This failif is filtered for readability.
		fail_if(info.reserved_leader_thread != 0, "Error checking namespace");
	}

	printf("Expected: %d\n", info.reserved_leader_thread);

	return 0;
}


