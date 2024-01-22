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

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>


void show(const int *a, int n)
{
    printf("Show %p size %d\n", a, n);
    for (int j=0; j<n; j++)
    {
        printf("0x%08x\n", a[j]);
    }
}

void check(const int *a, const int *ref, int n, const char *name)
{
    // printf("Check %p size %d\n", a, n);
    for (int j=0; j<n; j++)
    {
        if (a[j] != ref[j])
        {
            const char *eq = (a[j] == ref[j]) ? "==" : "!=";
            printf("%s[%d]: 0x%08x %s 0x%08x\n", name, j, a[j], eq, ref[j]);
            assert_that(a[j] == ref[j]);
        }
    }
}

void copy(int *a, const int *ref, int n)
{
    memcpy(a, ref, n * sizeof(int));
}

void check_hash(const int *a, int hash, int n, const char *name)
{
    int val = 0;
    for (int j=0; j<n; j++)
    {
        val ^= a[j];
        val = ((val * 1103515245) + 12345) & 0x7fffffff;
    }
    if (val != hash)
        printf("Error in %s\n", name);
    assert_that(val == hash);
}

void fill(int *a, int val, int n)
{
    for (int j=0; j<n; j++)
    {
        a[j] = val;
        val = ((val * 1103515245) + 12345) & 0x7fffffff;
    }
}


int out_1[1] = {
	0xfe83dd97
};
int out_2[1] = {
	0xc546b1ff
};
int out_3[1] = {
	0xef38529e
};
int out_4[1] = {
	0x8a00a4d3
};
void func1(int *b)
{
}

void func3(int *b)
{
}

void func8(int *b)
{
}

#pragma oss task inout(b[567;229]) node(1) label("task0")
void func0(int *b)
{
	#pragma oss task weakinout(b[685;105]) in(b[791;2]) in(b[643;16]) inout(b[598;13]) node(2) label("func1")
    func1(b);
}


#pragma oss task weakout(b[465;209]) weakin(b[310;31]) weakin(b[263;17]) node(2) label("task1")
void func23(int *b)
{
}
void func24(int *b)
{
}
void func25(int *b)
{
}
#pragma oss task weakinout(b[294;39]) weakout(b[696;115]) weakout(b[811;41]) node(0) label("task2")
void func26(int *b)
{
}
#pragma oss task out(b[560;212]) weakinout(b[265;31]) inout(b[312;17]) node(2) label("task3")
void func27(int *b)
{
}
#pragma oss task inout(b[299;20]) weakout(b[698;132]) out(b[830;22]) node(2) label("task4")
void func28(int *b)
{
}
void func29(int *b)
{
}

#pragma oss task out(b[453;400]) inout(b[248;93]) node(1) label("task5")
void func22(int *b)
{
	#pragma oss task inout(b[254;11]) out(b[846;4]) out(b[850;3]) weakout(b[841;3]) node(1) label("func24")
    func24(b);

    b[789] = 0xfe83dd97;

    //#pragma oss task weakout(b[778;48]) weakout(b[682;58]) inout(b[272;43]) node(2) label("task6")
    //func25(b);

    func26(b);

    func27(b);

    func28(b);

	#pragma oss task out(b[560;223]) out(b[830;17]) weakout(b[852;1]) node(1) label("func29")
    func29(b);
}

#pragma oss task inout(b[792;1]) in(b[794;1]) out(a[336;16]) out(b[791;1]) node(2) label("task7")
void func32(int *a,int *b)
{
}
#pragma oss task weakinout(b[794;1]) inout(b[790;4]) out(a[397;156]) node(0) label("task8")
void func33(int *a,int *b)
{
}
#pragma oss task out(b[793;1]) inout(b[792;1]) weakout(a[440;46]) node(0) label("task9")
void func34(int *a,int *b)
{
}
#pragma oss task out(a[351;122]) out(a[473;90]) out(b[794;1]) out(a[568;1]) node(2) label("task10")
void func35(int *a,int *b)
{
}
#pragma oss task weakin(b[791;1]) weakin(b[793;1]) in(b[789;1]) node(0) label("task11")
void func36(int *b)
{
    assert_that(b[789] == 0xfe83dd97);
}

#pragma oss task weakout(a[297;275]) weakinout(b[786;9]) node(0) label("task12")
void func31(int *a,int *b)
{
    func32(a,b);

    func33(a,b);

    func34(a,b);

    func35(a,b);

    func36(b);  // this fails
}

bool fail9()
{
    int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(1000 * sizeof(int));

    b[789] = -1;
    func0(b);  // needed for failure

    func22(b);

    func31(a,b);

	#pragma oss taskwait
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail9);

	return run_test(&test);
}
