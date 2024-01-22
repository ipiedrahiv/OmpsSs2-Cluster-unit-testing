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


bool test_local_ranges()
{
	size_t tasks_nr = ARRAY_SZ / TASK_SZ;

	int *buff = array_alloc_int(ARRAY_SZ, true);
	int *sum = array_alloc_int(tasks_nr, true);
	bool success = false;


	#pragma oss task out(buff[0;ARRAY_SZ]) label("init_buff_range")
	init_buff_range_int(buff, ARRAY_SZ, 1);

	#pragma oss task out(sum[0;tasks_nr]) label("init_buff_zeros")
	init_buff_value_int(sum, tasks_nr, 0);

	#pragma oss task in(buff[0;ARRAY_SZ]) inout(sum[0;tasks_nr]) label("sum_decompose")
	sum_decompose(buff, ARRAY_SZ, sum, TASK_SZ);

	#pragma oss task in(sum[0;tasks_nr]) out(success) label("test_result")
	test_result(sum, tasks_nr, &success, EXPECTED_VALUE);

	#pragma oss taskwait
	array_free_int(sum, tasks_nr, true);
	array_free_int(buff, ARRAY_SZ, true);

	return success;
}


int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(test_local_ranges);

	return run_test(&test);
}
