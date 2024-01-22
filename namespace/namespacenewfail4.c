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




bool namespacenewfail4()
{
	int a[1];
	#pragma oss task in(a) label("a") node(0) wait
	{
		#pragma oss task in(a) label("b") node(0)
		{
			sleep(1);
		} 
	}

	#pragma oss taskwait

	return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(namespacenewfail4);

	return run_test(&test);
}
