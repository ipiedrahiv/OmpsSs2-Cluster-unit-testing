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

	const int size = nanos6_get_num_cluster_nodes();

	fail_if(info.cluster_num_nodes != size, "Error getting cluster_num_nodes");
	fail_if(info.cluster_num_min_nodes != size, "Error getting cluster_num_min_nodes");
	fail_if(info.cluster_num_max_nodes != size, "Error getting cluster_num_max_nodes");
	fail_if(info.malleability_enabled != 0, "Error getting malleability_enabled");

	fail_if(info.namespace_enabled != nanos6_get_namespace_is_enabled(), "Error checking namespace");

	fail_if(info.merge_release_and_finish == 0, "Error checking merge_release_and_finish");


	return 0;
}


