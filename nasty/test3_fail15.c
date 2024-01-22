/*
 * Copyright (C) 2022  Paul Capenter
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


/*
 * Test case far the following commit of the cluster_develop_paul_bugfix
 * branch of Nanos6@cluster:
 *
 * 90f4400c Updated fix to taskwait in cluster mode
 */

void func271(int *b)
{
}

void func268(int *b)
{
    #pragma oss task in(b[3;10]) node(1) label("func271")
    func271(b);

    #pragma oss taskwait
}

void func282(int *b)
{
}

void func301(int *b)
{
    b[2] = 1; // right value
}

void func260(int *b)
{
    #pragma oss task in(b[0;25]) node(2) label("func268")
    func268(b);

    #pragma oss task out(b[5;20]) node(2) label("func282")
    func282(b);

    #pragma oss task inout(b[2;3]) node(1) label("func301")
    func301(b);   // writes the real value
}

bool fail15(void)
{
	int *b = nanos6_lmalloc(25 * sizeof(int));
	int ref = 1;

    b[2] = -1; // wrong value

    #pragma oss task weakinout(b[0;25]) node(2) label("func260")
    func260(b);

    #pragma oss taskwait

	printf("b[2] at %p: 0x%08x %s 0x%08x\n", &b[2], b[2], (b[2] == ref) ? "==" : "!=", ref);
	fail_if(b[2] != ref, "Check failed\n");

	return (b[2] == ref);
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail15);

	return run_test(&test);
}
