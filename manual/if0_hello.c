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


bool hello_world()
{
	int *ret = array_alloc_int(1, false);

	#pragma oss task out(ret[0]) label("hello_world") if(0)
	*ret = -42;

	#pragma oss taskwait

	bool success = (*ret == -42);

	array_free_int(ret, 1, false);

	return success;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(hello_world);

	return run_test(&test);
}
