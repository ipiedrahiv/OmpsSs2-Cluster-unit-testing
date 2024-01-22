/*
 * Copyright (C) 2021  Jimmy Aguilar Mena
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
#include <limits.h>

int main(int argc, char *argv[])
{
	// maxsize is 2^31 so the array will be 4 * (2^31)
	/* size_t array_sz = (size_t)(INT_MAX) + 8; */
	size_t array_sz = 256;

	size_t *buff = array_alloc_size_t(array_sz, false);

	#pragma oss task out(buff[0;array_sz]) node(1) label("init_buff")
	{
		printf("Initializing array sz = %zu\n", array_sz);
		init_buff_range_size_t(buff, array_sz, 0);
		printf("Done initialization\n");

		for (size_t i = 0; i < array_sz; ++i) {
			fail_if(buff[i] != i, "Error in assertion0: ");
		}
		printf("Initialization is fine\n");
	}

	#pragma oss task inout(buff[0;array_sz]) node(0) label("check_0")
	{
		printf("Start task 0\n");
		for (size_t i = 0; i < array_sz; ++i) {
			fail_if(buff[i] != i, "Error in assertion1: ");
			buff[i] = array_sz - i;
		}
		printf("Done task 0\n");
	}

	#pragma oss task in(buff[0;array_sz]) node(1) label("check_1")
	{
		printf("Start task 1\n");
		for (size_t i = 0; i < array_sz; ++i) {
			fail_if(buff[i] != array_sz - i, "Error in assertion2: ");
		}
		printf("Done task 1\n");
	}

	#pragma oss taskwait

	array_free_size_t(buff, array_sz, false);

    return 0;
}
