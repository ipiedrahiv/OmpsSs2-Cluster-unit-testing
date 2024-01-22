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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * test4_executetask.c:
 *
 * This program causes an assertion failure in ExecutionWorkflow::executeTask:
 * Assertion `executionStep != nullptr' failed. This was fixed in the following
 * commit of the cluster_develop_dlb_paul branch of Nanos6@cluster:
 *
 *  73d855f1 Fix for executeTask after remote-task-wrapper finishes
 *
 */

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanos6.h>



void func3(int *b)
{
}
void func4(int *b)
{
}
void func5(int *b)
{
}
void func6(int *b)
{
}
void func2(int *b)
{
    #pragma oss task weakin(b[914;1]) in(b[915;2]) node(1) label("func3")
    func3(b);

    #pragma oss task weakin(b[914;1]) in(b[917;1]) node(2) label("func4")
    func4(b);

    #pragma oss task weakin(b[917;1]) node(3) label("func5")
    func5(b);

    #pragma oss task in(b[914;1]) in(b[917;1]) node(1) label("func6")
    func6(b);

}
void func8(int *b)
{
}
void func9(int *b)
{
}
void func10(int *b)
{
}
void func11(int *b)
{
}
void func7(int *b)
{
    #pragma oss task in(b[914;1]) in(b[917;1]) node(2) label("func8")
    func8(b);

    #pragma oss task in(b[916;1]) in(b[914;1]) in(b[917;1]) node(0) label("func9")
    func9(b);

    #pragma oss task in(b[917;1]) weakin(b[914;1]) weakin(b[916;1]) node(1) label("func10")
    func10(b);

    #pragma oss task weakin(b[917;1]) weakin(b[914;1]) node(0) label("func11")
    func11(b);

}
void func13(int *b)
{
}
void func14(int *b)
{
}
void func15(int *b)
{
}
void func16(int *b)
{
}
void func12(int *b)
{
    #pragma oss task weakin(b[917;1]) weakin(b[916;1]) in(b[914;1]) node(3) label("func13")
    func13(b);

    #pragma oss task weakin(b[914;1]) in(b[917;1]) weakin(b[916;1]) node(1) label("func14")
    func14(b);

    #pragma oss task in(b[917;1]) in(b[914;1]) node(1) label("func15")
    func15(b);

    #pragma oss task weakin(b[917;1]) weakin(b[914;1]) node(0) label("func16")
    func16(b);

}
void func18(int *b)
{
}

void func17(int *b)
{
    #pragma oss task in(b[916;1]) node(3) label("func18")
    func18(b);
}

void func1(int *b)
{
    #pragma oss task in(b[914;1]) in(b[917;1]) weakin(b[915;2]) node(1) label("func2")
    func2(b);

    #pragma oss task in(b[916;2]) in(b[914;1]) node(3) label("func7")
    func7(b);

    #pragma oss task in(b[917;1]) in(b[914;1]) weakin(b[915;2]) node(3) label("func12")
    func12(b);

    #pragma oss task weakin(b[916;1]) node(3) label("func17")
    func17(b);

}
void func24(int *b)
{
}
void func25(int *b)
{
}
void func26(int *b)
{
}
void func27(int *b)
{
}
void func23(int *b)
{
    #pragma oss task in(b[890;1]) weakin(b[899;1]) out(b[912;1]) out(b[911;1]) node(2) label("func24")
    func24(b);

    #pragma oss task in(b[909;2]) out(b[912;1]) in(b[893;2]) weakin(b[895;1]) node(3) label("func25")
    func25(b);

    #pragma oss task weakin(b[909;2]) in(b[895;1]) node(1) label("func26")
    func26(b);

    #pragma oss task weakout(b[911;1]) in(b[909;1]) weakin(b[892;8]) node(1) label("func27")
    func27(b);

}
void func29(int *b)
{
}
void func30(int *b)
{
}
void func31(int *b)
{
}
void func32(int *b)
{
}
void func28(int *b)
{
    #pragma oss task in(b[913;1]) weakin(b[907;1]) in(b[892;1]) node(1) label("func29")
    func29(b);

    #pragma oss task weakin(b[907;1]) in(b[894;2]) weakin(b[896;2]) node(2) label("func30")
    func30(b);

    #pragma oss task weakin(b[892;1]) weakin(b[907;1]) in(b[893;2]) node(3) label("func31")
    func31(b);

    #pragma oss task in(b[907;1]) weakin(b[913;1]) node(3) label("func32")
    func32(b);

}
void func34(int *b)
{
}
void func35(int *b)
{
}
void func36(int *b)
{
}
void func37(int *b)
{
}
void func33(int *b)
{
    #pragma oss task in(b[913;1]) node(3) label("func34")
    func34(b);

    #pragma oss task in(b[913;1]) node(1) label("func35")
    func35(b);

    #pragma oss task weakin(b[913;1]) node(0) label("func36")
    func36(b);

    #pragma oss task in(b[913;1]) node(0) label("func37")
    func37(b);

}
void func39(int *b)
{
}
void func40(int *b)
{
}
void func41(int *b)
{
}
void func42(int *b)
{
}
void func38(int *b)
{
    #pragma oss task weakout(b[910;1]) in(b[885;1]) node(1) label("func39")
    func39(b);

    #pragma oss task weakout(b[910;1]) weakin(b[886;1]) in(b[887;1]) node(0) label("func40")
    func40(b);

    #pragma oss task out(b[910;1]) weakin(b[884;3]) in(b[887;1]) node(3) label("func41")
    func41(b);

    #pragma oss task in(b[882;6]) node(2) label("func42")
    func42(b);

}
void func22(int *b)
{
    #pragma oss task out(b[911;2]) weakin(b[909;2]) weakin(b[889;11]) node(1) label("func23")
    func23(b);

    #pragma oss task in(b[890;8]) in(b[913;1]) weakin(b[907;1]) node(0) label("func28")
    func28(b);

    #pragma oss task weakin(b[913;1]) node(0) label("func33")
    func33(b);

    #pragma oss task weakin(b[882;6]) weakinout(b[910;1]) node(3) label("func38")
    func38(b);

}

void func66(int *b)
{
}
void func67(int *b)
{
}
void func68(int *b)
{
}
void func69(int *b)
{
}
void func65(int *b)
{
    #pragma oss task weakin(b[838;1]) weakin(b[837;1]) node(1) label("func66")
    func66(b);

    #pragma oss task in(b[837;1]) in(b[838;1]) node(1) label("func67")
    func67(b);

    #pragma oss task weakin(b[837;1]) in(b[838;1]) node(1) label("func68")
    func68(b);

    #pragma oss task weakin(b[837;1]) node(0) label("func69")
    func69(b);

}
void func71(int *b)
{
}
void func72(int *b)
{
}
void func73(int *b)
{
}
void func74(int *b)
{
}
void func70(int *b)
{
    #pragma oss task in(b[838;1]) weakin(b[837;1]) node(0) label("func71")
    func71(b);

    #pragma oss task weakin(b[837;1]) weakin(b[838;1]) node(2) label("func72")
    func72(b);

    #pragma oss task weakin(b[837;2]) node(1) label("func73")
    func73(b);

    #pragma oss task weakin(b[838;1]) weakin(b[837;1]) node(0) label("func74")
    func74(b);

}
void func76(int *b)
{
}
void func77(int *b)
{
}
void func78(int *b)
{
}
void func79(int *b)
{
}
void func75(int *b)
{
    #pragma oss task in(b[838;1]) weakin(b[837;1]) node(0) label("func76")
    func76(b);

    #pragma oss task weakin(b[837;1]) in(b[838;1]) node(2) label("func77")
    func77(b);

    #pragma oss task weakin(b[837;1]) weakin(b[838;1]) node(3) label("func78")
    func78(b);

    #pragma oss task weakin(b[837;1]) weakin(b[838;1]) node(1) label("func79")
    func79(b);

}
void func81(int *b)
{
}
void func82(int *b)
{
}
void func83(int *b)
{
}
void func84(int *b)
{
}
void func80(int *b)
{
    #pragma oss task weakin(b[838;1]) node(1) label("func81")
    func81(b);

    #pragma oss task in(b[838;1]) node(3) label("func82")
    func82(b);

    #pragma oss task in(b[838;1]) node(2) label("func83")
    func83(b);

    #pragma oss task in(b[838;1]) node(2) label("func84")
    func84(b);

}
void func64(int *b)
{
    #pragma oss task in(b[837;1]) in(b[838;1]) node(2) label("func65")
    func65(b);

    #pragma oss task in(b[837;2]) node(1) label("func70")
    func70(b);

    #pragma oss task in(b[837;1]) weakin(b[838;1]) node(1) label("func75")
    func75(b);

    #pragma oss task in(b[838;1]) node(1) label("func80")
    func80(b);

}
void func0(int *b)
{
    #pragma oss task weakin(b[914;4]) node(0) label("func1")
    func1(b);

    #pragma oss task weakout(b[833;1]) in(b[882;23]) weakinout(b[905;11]) node(0) label("func22")
    func22(b);

    #pragma oss task weakin(b[837;2]) node(3) label("func64")
    func64(b);
}

bool fail10()
{
    int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(1000 * sizeof(int));


    #pragma oss task out(b[879;54]) out(b[820;19]) node(0) label("func0")
    func0(b);

    #pragma oss taskwait
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail10);

	return run_test(&test);
}
