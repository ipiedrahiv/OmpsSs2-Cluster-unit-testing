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


bool local_reduce()
{
	int *buff = array_alloc_int(ARRAY_SZ, false);

	size_t tasks_nr = ARRAY_SZ / TASK_SZ;

	int *sum = array_alloc_int(tasks_nr, false);

	#pragma oss task out(buff[0;ARRAY_SZ]) label("init_range")
	init_buff_range_int(buff, ARRAY_SZ, 1);

	#pragma oss task out(sum[0;tasks_nr]) label("init_zeros")
	init_buff_value_int(sum, tasks_nr, 0);

	for (size_t i = 0; i < ARRAY_SZ; i += TASK_SZ) {
		#pragma oss task in(buff[i;TASK_SZ]) inout(sum[i/TASK_SZ]) label("sum")
		sum_task(&buff[i], TASK_SZ, &sum[i / TASK_SZ]);
	}

	#pragma oss task inout(sum[0;tasks_nr]) label("reduction_if0") if(0)
	{
		for (size_t i = 0; i < tasks_nr - 1; ++i)
			sum[tasks_nr - 1] += sum[i];
	}

	#pragma oss taskwait

	/* side-effect: sum should be on this node since the 'reduction_if0'
	 * must execute on this node and bring all data in. As a result should
	 * be able to read it without enclosing it into a task */

#include "util.h"

	bool ret = (sum[tasks_nr - 1] == EXPECTED_VALUE);

	array_free_int(sum, tasks_nr, false);
	array_free_int(buff, ARRAY_SZ, false);

	return ret;
}


int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(local_reduce);

	return run_test(&test);
}
