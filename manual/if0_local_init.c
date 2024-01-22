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


bool local_init()
{
	int *buff = array_alloc_int(ARRAY_SZ, false);

	size_t tasks_nr = ARRAY_SZ / TASK_SZ;

	int *sum = array_alloc_int(tasks_nr, true);

	#pragma oss task out(buff[0;ARRAY_SZ]) label("init_range_if0") if(0)
	init_buff_range_int(buff, ARRAY_SZ, 1);

	#pragma oss task out(sum[0;tasks_nr]) label("init_zeros_if0") if(0)
	init_buff_value_int(sum, tasks_nr, 0);

	for (size_t i = 0; i < ARRAY_SZ; i += TASK_SZ) {
		#pragma oss task in(buff[i;TASK_SZ]) inout(sum[i/TASK_SZ]) label("task0")
		sum_task(&buff[i], TASK_SZ, &sum[i / TASK_SZ]);
	}

	#pragma oss task inout(sum[0;tasks_nr]) label("reduction")
	for (size_t i = 1; i < tasks_nr; ++i)
		sum[0] += sum[i];

	#pragma oss taskwait

	int _sum = sum[0];

	array_free_int(sum, tasks_nr, true);
	array_free_int(buff, ARRAY_SZ, false);

	return _sum == EXPECTED_VALUE;
}


int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(local_init);

	return run_test(&test);
}
