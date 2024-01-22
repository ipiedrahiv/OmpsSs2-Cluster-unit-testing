// Generated by ./nasty.py --nodes 3 --tasks 70 --nesting 3 --seed 128

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

void check(const int *a, const int *ref, int n, const char *name, int ofs)
{
	// printf("Check %p size %d\n", a, n);
	for (int j=0; j<n; j++)
	{
		if (a[j] != ref[j])
		{
			const char *eq = (a[j] == ref[j]) ? "==" : "!=";
			printf("%s[%d]: 0x%08x %s 0x%08x\n", name, ofs+j, a[j], eq, ref[j]);
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


void func2(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[882], 0x73127f9d, 4);
}
void func3(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[884], 0x230cc2a4, 2);
}
void func4(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[885], 0x739f0159, 1);
}
void func5(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[885], 0x46ff6cd5, 1, "check_5");
    fill(&b[885], 0x5c831be2, 1);
}
void func1(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    #pragma oss task out(b[882;4]) node(2) label("func2")
    func2(b);

    #pragma oss task out(b[884;2]) weakout(b[883;1]) node(0) label("func3")
    func3(b);

    #pragma oss task out(b[885;1]) node(0) label("func4")
    func4(b);

    #pragma oss taskwait on(b[885;1])

    fill(&b[885], 0xa4121d8c, 1);
    #pragma oss task inout(b[885;1]) node(1) label("func5")
    func5(b);

}
void func7(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&b[584], 0x5d0e0503, 70);
}
void func8(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[634], 0x4ee5b834, 2);
    fill(&b[636], 0x9173829c, 10);
}
void func9(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&b[628], 0x2c5b5bd0, 18);
}
void func10(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[639], 0x220bff41, 10);
    fill(&b[601], 0xdebc7f28, 18);
}
void func6(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss taskwait

    #pragma oss taskwait on(b[616;31])

    fill(&b[616], 0x01bffe78, 31);
    #pragma oss taskwait on(b[595;2])

    fill(&b[595], 0x25fee17e, 2);
    #pragma oss taskwait

    check_hash(&b[595], 0x00003039, 2, "check_9");
    check_hash(&b[616], 0x358cd338, 31, "check_9");
    #pragma oss task out(b[584;70]) node(1) label("func7")
    func7(b);

    #pragma oss task out(b[634;2]) out(b[636;10]) node(0) label("func8")
    func8(b);

    #pragma oss task out(b[628;18]) node(1) label("func9")
    func9(b);

    #pragma oss task out(b[639;10]) out(b[601;18]) node(0) label("func10")
    func10(b);

}
void func12(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func13(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
}
void func14(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[874], 0x36e51aa5, 1);
}
void func15(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&b[874], 0xc27561c3, 1);
}
void func11(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss task weakout(b[168;41]) weakout(b[209;14]) node(0) label("func12")
    func12(b);

    #pragma oss task weakout(b[181;12]) node(1) label("func13")
    func13(b);

    #pragma oss task out(b[874;1]) weakout(b[875;2]) node(2) label("func14")
    func14(b);

    #pragma oss taskwait

    #pragma oss task out(b[874;1]) weakout(b[876;1]) node(1) label("func15")
    func15(b);

}
void func17(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[330], 0x00003039, 2, "check_13");
    check_hash(&b[465], 0x1675bd81, 3, "check_14");
    fill(&b[465], 0xacd032cc, 3);
}
void func18(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[326], 0xb2376564, 2);
}
void func19(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[464], 0x45edd176, 3, "check_19");
    check_hash(&b[329], 0x5c7c3459, 3, "check_20");
    fill(&b[464], 0x1fe5524b, 3);
    fill(&b[329], 0x30d56f3d, 3);
}
void func20(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[470], 0x0cc6858d, 1, "check_21");
}
void func16(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss task weakout(b[327;2]) weakin(b[456;1]) in(b[330;2]) inout(b[465;3]) node(1) label("func17")
    func17(b);

    #pragma oss task weakinout(b[469;1]) weakout(b[455;1]) out(b[326;2]) node(2) label("func18")
    func18(b);

    #pragma oss taskwait

    #pragma oss taskwait

    #pragma oss task inout(b[464;3]) inout(b[329;3]) weakout(b[453;7]) node(1) label("func19")
    func19(b);

    #pragma oss task weakout(b[313;14]) weakout(b[312;1]) weakinout(b[333;3]) in(b[470;1]) node(2) label("func20")
    func20(b);

}
void func0(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss task weakout(b[882;4]) node(1) label("func1")
    func1(b);

    #pragma oss task weakout(b[568;86]) node(2) label("func6")
    func6(b);

    #pragma oss task weakout(b[160;67]) out(b[871;6]) node(2) label("func11")
    func11(b);

    #pragma oss taskwait on(b[132;397])

    fill(&b[132], 0x6037dd46, 397);
    #pragma oss task weakinout(b[448;23]) weakinout(b[312;24]) node(2) label("func16")
    func16(b);

}
void func23(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&b[811], 0xdb113a82, 12);
}
void func24(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[823], 0x5d7f8137, 1, "check_28");
    fill(&b[831], 0xde5ca9f2, 13);
    fill(&b[823], 0xca2e49bd, 1);
}
void func25(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func26(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
}
void func22(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&b[847], 0x36b05e25, 10);
    #pragma oss taskwait

    check_hash(&b[847], 0x354e0aa9, 10, "check_24");
    fill(&b[798], 0x74863529, 26);
    #pragma oss taskwait

    check_hash(&b[798], 0x55241d79, 26, "check_26");
    check_hash(&b[847], 0x354e0aa9, 10, "check_26");
    #pragma oss task out(b[811;12]) weakin(b[850;1]) weakinout(b[823;1]) node(1) label("func23")
    func23(b);

    fill(&b[835], 0x87af3fbd, 15);
    #pragma oss task weakinout(b[798;7]) out(b[831;13]) weakout(b[849;9]) inout(b[823;1]) node(0) label("func24")
    func24(b);

    fill(&b[785], 0xd2b19833, 4);
    #pragma oss task weakin(b[787;2]) weakout(b[842;14]) weakout(b[830;2]) weakout(b[809;21]) node(0) label("func25")
    func25(b);

    #pragma oss task weakinout(b[785;4]) weakout(b[812;35]) node(2) label("func26")
    func26(b);

}
void func28(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[747], 0x50a53844, 85);
}
void func29(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
}
void func30(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[804], 0x1650edc1, 1);
}
void func31(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[730], 0x7e736520, 16, "check_36");
    fill(&b[730], 0x727a28b1, 16);
}
void func27(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    #pragma oss taskwait

    #pragma oss taskwait

    #pragma oss task weakout(b[726;21]) out(b[747;85]) node(0) label("func28")
    func28(b);

    fill(&b[832], 0xe40b42f9, 1);
    #pragma oss taskwait on(b[730;16])

    fill(&b[730], 0xc3e6f28c, 16);
    #pragma oss task weakin(b[832;1]) weakinout(b[731;15]) node(2) label("func29")
    func29(b);

    #pragma oss taskwait on(b[756;39])

    fill(&b[756], 0x29ca31f5, 39);
    #pragma oss task weakout(b[823;10]) out(b[804;1]) node(2) label("func30")
    func30(b);

    #pragma oss taskwait on(b[822;10])

    fill(&b[822], 0x4df9cfdf, 10);
    #pragma oss task weakin(b[823;1]) weakinout(b[770;25]) inout(b[730;16]) node(2) label("func31")
    func31(b);

}
void func33(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[91], 0x35fce55a, 1, "check_37");
}
void func34(int *a)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&a[860], 0xe4bc4cb2, 1);
}
void func35(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[91], 0x35fce55a, 1, "check_38");
    fill(&a[861], 0x9ecd0c94, 1);
}
void func36(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    fill(&a[860], 0x351e746d, 1);
}
void func32(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss task in(b[91;1]) weakout(a[851;1]) node(1) label("func33")
    func33(a,b);

    #pragma oss task out(a[860;1]) node(1) label("func34")
    func34(a);

    #pragma oss task out(a[861;1]) in(b[91;1]) node(1) label("func35")
    func35(a,b);

    #pragma oss task out(a[860;1]) weakin(b[91;1]) node(1) label("func36")
    func36(a,b);

}
void func38(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[95], 0x6e13fd26, 1, "check_39");
}
void func39(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[95], 0x6e13fd26, 1, "check_40");
}
void func40(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
}
void func41(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[96], 0x67088d67, 1, "check_44");
}
void func37(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    #pragma oss task weakin(b[96;1]) in(b[95;1]) node(2) label("func38")
    func38(b);

    #pragma oss task weakin(b[96;1]) in(b[95;1]) weakin(b[93;1]) node(2) label("func39")
    func39(b);

    #pragma oss taskwait on(b[93;4])

    check_hash(&b[93], 0x7ee61f24, 4, "check_41");
    #pragma oss taskwait

    check_hash(&b[93], 0x7ee61f24, 4, "check_42");
    #pragma oss task weakin(b[93;2]) weakin(b[95;1]) weakin(b[96;1]) node(1) label("func40")
    func40(b);

    #pragma oss taskwait on(b[94;1])

    check_hash(&b[94], 0x08326881, 1, "check_43");
    #pragma oss task weakin(b[94;2]) in(b[96;1]) node(1) label("func41")
    func41(b);

}
void func21(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[90], 0x50b076e0, 8, "check_22");
    #pragma oss task out(b[784;76]) node(1) label("func22")
    func22(b);

    #pragma oss task out(b[724;110]) node(0) label("func27")
    func27(b);

    #pragma oss task weakin(b[91;1]) out(a[848;14]) node(2) label("func32")
    func32(a,b);

    #pragma oss task weakin(b[93;4]) node(1) label("func37")
    func37(b);

}
void func44(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[92], 0x482b438b, 1, "check_46");
}
void func45(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[95], 0x6e13fd26, 1, "check_47");
    fill(&b[91], 0x1ad3654c, 1);
}
void func46(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[96], 0x67088d67, 1, "check_48");
    check_hash(&b[92], 0x482b438b, 1, "check_49");
    fill(&b[96], 0xac64a50c, 1);
}
void func47(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
}
void func43(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[93], 0x2ac80691, 3, "check_45");
    #pragma oss task in(b[92;1]) weakout(b[91;1]) node(0) label("func44")
    func44(b);

    #pragma oss task weakin(b[96;1]) in(b[95;1]) out(b[91;1]) node(2) label("func45")
    func45(b);

    #pragma oss task inout(b[96;1]) in(b[92;1]) node(0) label("func46")
    func46(b);

    #pragma oss taskwait on(b[93;1])

    #pragma oss task weakin(b[96;1]) weakin(b[93;2]) weakout(b[91;1]) node(0) label("func47")
    func47(b);

}
void func49(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[96], 0x446c1e55, 1, "check_51");
    check_hash(&b[92], 0x4e15ccb6, 3, "check_52");
}
void func50(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[96], 0x446c1e55, 1, "check_58");
}
void func51(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[96], 0x446c1e55, 1, "check_65");
}
void func52(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[96], 0x446c1e55, 1, "check_67");
    check_hash(&b[97], 0x7e63c714, 1, "check_68");
    check_hash(&b[92], 0x482b438b, 1, "check_69");
}
void func48(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    #pragma oss task in(b[96;1]) weakin(b[97;1]) in(b[92;3]) node(2) label("func49")
    func49(b);

    #pragma oss taskwait on(b[97;1])

    check_hash(&b[97], 0x7e63c714, 1, "check_53");
    #pragma oss taskwait

    check_hash(&b[97], 0x7e63c714, 1, "check_56");
    #pragma oss taskwait on(b[96;1])

    check_hash(&b[96], 0x446c1e55, 1, "check_57");
    #pragma oss task weakin(b[93;1]) weakin(b[97;1]) in(b[96;1]) node(2) label("func50")
    func50(b);

    #pragma oss taskwait

    #pragma oss taskwait on(b[96;1])

    check_hash(&b[96], 0x446c1e55, 1, "check_62");
    #pragma oss taskwait on(b[96;1])

    #pragma oss taskwait on(b[94;1])

    check_hash(&b[94], 0x08326881, 1, "check_64");
    #pragma oss task weakin(b[97;1]) weakin(b[92;3]) in(b[96;1]) node(1) label("func51")
    func51(b);

    #pragma oss taskwait on(b[92;3])

    check_hash(&b[92], 0x4e15ccb6, 3, "check_66");
    #pragma oss task in(b[96;1]) in(b[97;1]) in(b[92;1]) node(0) label("func52")
    func52(b);

}
void func54(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
}
void func55(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
}
void func56(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    fill(&b[95], 0x08ae478b, 1);
}
void func57(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[95], 0xd1fd5cfd, 1);
}
void func53(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[92], 0x4e15ccb6, 3, "check_72");
    check_hash(&b[95], 0x6e13fd26, 1, "check_73");
    fill(&b[92], 0xb070f8f2, 3);
    #pragma oss task weakout(b[96;1]) weakin(b[95;1]) weakout(b[93;1]) node(2) label("func54")
    func54(b);

    #pragma oss taskwait on(b[93;1])

    fill(&b[93], 0xab5a4629, 1);
    #pragma oss taskwait on(b[92;2])

    #pragma oss taskwait on(b[92;1])

    #pragma oss taskwait

    check_hash(&b[92], 0x5975f738, 3, "check_79");
    check_hash(&b[95], 0x6e13fd26, 1, "check_80");
    #pragma oss task weakin(b[95;1]) weakin(b[92;3]) node(2) label("func55")
    func55(b);

    #pragma oss task out(b[95;1]) weakout(b[96;1]) node(0) label("func56")
    func56(b);

    #pragma oss task out(b[95;1]) weakinout(b[94;1]) node(2) label("func57")
    func57(b);

}
void func59(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[93], 0x23868dae, 1, "check_83");
}
void func60(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[97], 0x7e63c714, 1, "check_84");
    fill(&b[93], 0x8d20876b, 1);
}
void func61(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
}
void func62(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    check_hash(&b[97], 0x7e63c714, 1, "check_89");
}
void func58(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    #pragma oss task in(b[93;1]) weakin(b[97;1]) node(2) label("func59")
    func59(b);

    #pragma oss task in(b[97;1]) weakout(b[92;1]) out(b[93;1]) node(2) label("func60")
    func60(b);

    #pragma oss taskwait

    fill(&b[96], 0xdb3e9ff2, 1);
    #pragma oss task weakout(b[96;1]) weakout(b[92;2]) node(1) label("func61")
    func61(b);

    #pragma oss task in(b[97;1]) node(1) label("func62")
    func62(b);

}
void func42(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    #pragma oss task weakinout(b[96;1]) weakout(b[91;1]) weakin(b[92;1]) in(b[93;3]) node(1) label("func43")
    func43(b);

    #pragma oss task weakin(b[96;1]) weakin(b[92;3]) weakin(b[97;1]) node(0) label("func48")
    func48(b);

    #pragma oss taskwait on(b[90;1])

    check_hash(&b[90], 0x59d1c105, 1, "check_70");
    fill(&b[90], 0xa56eab97, 1);
    #pragma oss taskwait

    check_hash(&b[90], 0x36314184, 1, "check_71");
    #pragma oss task inout(b[92;3]) inout(b[95;1]) weakout(b[96;1]) node(1) label("func53")
    func53(b);

    #pragma oss taskwait

    check_hash(&b[90], 0x36314184, 1, "check_82");
    #pragma oss task weakin(b[97;1]) weakinout(b[92;2]) out(b[96;1]) node(0) label("func58")
    func58(b);

}
void func65(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[284], 0x00003039, 2, "check_93");
    check_hash(&b[286], 0x35087f95, 17, "check_94");
}
void func66(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[291], 0x00003039, 2, "check_95");
}
void func67(int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    fill(&b[682], 0xc8a1a87e, 1);
}
void func68(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
}
void func64(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 2);
    check_hash(&b[276], 0x216e60d7, 33, "check_92");
    #pragma oss task in(b[284;2]) in(b[286;17]) node(2) label("func65")
    func65(b);

    #pragma oss task in(b[291;2]) node(0) label("func66")
    func66(b);

    #pragma oss task out(b[682;1]) weakin(b[285;22]) node(2) label("func67")
    func67(b);

    #pragma oss taskwait

    check_hash(&b[276], 0x216e60d7, 33, "check_97");
    #pragma oss task weakout(a[650;43]) weakin(b[291;18]) node(2) label("func68")
    func68(a,b);

}
void func69(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 0);
    check_hash(&b[280], 0x1dc833c0, 32, "check_99");
}
void func63(int *a,int *b)
{
   assert_that( nanos6_get_cluster_node_id() == 1);
    #pragma oss task out(a[609;162]) in(b[276;33]) out(b[681;2]) node(2) label("func64")
    func64(a,b);

    #pragma oss task weakinout(b[658;7]) weakout(a[793;46]) in(b[280;32]) node(0) label("func69")
    func69(a,b);

}


int main(void)
{
	int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
	int *b = nanos6_lmalloc(1000 * sizeof(int));

    fill(&b[0], 0x0166a6f3, 1000);

    #pragma oss taskwait

    fill(&b[90], 0x9b05647c, 334);
    #pragma oss task weakout(b[823;65]) out(b[98;445]) out(b[543;113]) node(2) label("func0")
    func0(b);

    #pragma oss task weakout(a[843;24]) in(b[90;8]) out(b[723;144]) node(1) label("func21")
    func21(a,b);

    #pragma oss task weakinout(b[90;8]) node(2) label("func42")
    func42(b);

    fill(&b[656], 0xe8d618c5, 67);
    #pragma oss taskwait on(a[887;8])

    fill(&a[887], 0x0c0bb7e3, 8);
    #pragma oss task weakinout(b[656;67]) weakout(a[508;360]) weakin(b[274;38]) node(1) label("func63")
    func63(a,b);

    #pragma oss taskwait

    check_hash(&a[887], 0x66be2bb8, 8, "check_100");
    check_hash(&b[90], 0x36314184, 1, "check_101");
    check_hash(&b[94], 0x6bdfe2f9, 1, "check_101");
    check_hash(&b[97], 0x7e63c714, 1, "check_101");
    check_hash(&b[132], 0x32b3c28c, 180, "check_101");
    check_hash(&b[329], 0x5bd85c0b, 95, "check_101");
    check_hash(&b[656], 0x01b1ecf2, 25, "check_101");
    check_hash(&b[683], 0x7fecd960, 40, "check_101");

	nanos6_dfree(a, 1000 * sizeof(int));
	nanos6_lfree(b, 1000 * sizeof(int));


    printf("Test passed [./nasty.py --nodes 3 --tasks 70 --nesting 3 --seed 128]\n");

    return(0);
}
