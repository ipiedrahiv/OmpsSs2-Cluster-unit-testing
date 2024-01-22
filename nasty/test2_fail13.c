/*
 * Copyright (C) 2020 Paul Carpenter and Omar Shaaban
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

/*
 * test_fail13.c:
 *
 * This program checks for an unusual case where Nanos6@cluster requires a
 * DataAccess with a fragment to be propagated via the data release step.  This
 * was fixed in the following commit of the cluster_develop_dlb_paul branch of
 * Nanos6@cluster:
 *
 *   a03e1b55 Fix for unusual case in clusters build (fail13 test case)
 */



#include "util.h"
#include <nanos6.h>

void func48(int *b)
{
}

void func53(int *b)
{
}

void func59(int *b)
{
    b[12] = 1;
}

void func58(int *b)
{
    #pragma oss task inout(b[12;1]) node(0) label("func59-inout-n2")
    func59(b);
}

void func42(int *b)
{
    #pragma oss task inout(b[12;1]) node(0) label("func48-n2")
    func48(b);

    #pragma oss task in(b[13;1]) node(1) label("func53-n1")
    func53(b);

    #pragma oss task weakinout(b[12;1]) node(1) label("func58-n1 (=>59)")
    func58(b);
}

bool fail13()
{
    int *b = nanos6_lmalloc(20 * sizeof(int));

    #pragma oss task inout(b[12;1]) in(b[13;1]) node(1) label("func42-n1")
    func42(b);

    #pragma oss taskwait

    if (b[12] != 1)
	{
		printf("Incorrect output b[12] = %d != %d\n", b[12], 1);
		return false;
	}

    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail13);

	return run_test(&test);
}
