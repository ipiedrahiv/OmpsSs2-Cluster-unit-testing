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

#include <stdio.h>
#include "util.h"

int main()
{
	int *a = malloc(8 * sizeof(int));

	for (int i  = 0; i < 8; ++ i) {
		#pragma oss task out(a[i])
		{
			a[i] = i;
		}
	}

	for (int i  = 0; i < 8; ++ i) {
		#pragma oss task in(a[i])
		{
			fail_if(a[i] != i, "Error a[%d] != %d\n", a[i], i);
		}
	}

	return 0;
}

