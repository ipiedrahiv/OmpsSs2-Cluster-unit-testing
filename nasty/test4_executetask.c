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


int out_0[4] = {
          0xfce173bc, 0x02ef35cc, 0xe2fe38d4, 0x7a03fb94
};
int ref_1[1] = {
          0xfce173bc
};
int ref_2[1] = {
          0x7a03fb94
};
int ref_3[2] = {
          0x02ef35cc, 0xe2fe38d4
};
int ref_4[2] = {
          0xe2fe38d4, 0x7a03fb94
};
int ref_5[1] = {
          0xe2fe38d4
};
int out_6[2] = {
          0x25a59706, 0xb25d2313
};
int out_7[32] = {
          0x3515f08a, 0xe2ebcbaf, 0xf797d327, 0x87802804,
          0x438d8a74, 0x8659bea1, 0x86fbb336, 0xe116cddd,
          0xe6779f12, 0x00793ef1, 0xcee7ad28, 0xd03c31f4,
          0x3b549cb7, 0x7c65c13d, 0x1e7384fd, 0x71e45be8,
          0x233d96b3, 0x28458a10, 0x5fbcbde4, 0xdf8673da,
          0x86524d9b, 0xf06ab87b, 0xebd2f257, 0xff66444f,
          0x5d202ab6, 0xe81cc943, 0x8fae6056, 0x5fb43e45,
          0x4ef44266, 0xb2392334, 0xc240b47c, 0xc7d3e1fc
};
int ref_8[23] = {
          0x3515f08a, 0xe2ebcbaf, 0xf797d327, 0x87802804,
          0x438d8a74, 0x8659bea1, 0x86fbb336, 0xe116cddd,
          0xe6779f12, 0x00793ef1, 0xcee7ad28, 0xd03c31f4,
          0x3b549cb7, 0x7c65c13d, 0x1e7384fd, 0x71e45be8,
          0x233d96b3, 0x28458a10, 0x5fbcbde4, 0xdf8673da,
          0x86524d9b, 0xf06ab87b, 0xebd2f257
};
int ref_9[1] = {
          0xe6779f12
};
int out_10[1] = {
          0x0c338da2
};
int out_11[1] = {
          0x8f612a37
};
int ref_12[2] = {
          0x5fb43e45, 0x4ef44266
};
int ref_13[2] = {
          0xd03c31f4, 0x3b549cb7
};
int out_14[1] = {
          0x3307409a
};
int ref_15[1] = {
          0x7c65c13d
};
int ref_16[1] = {
          0x5fb43e45
};
int ref_17[8] = {
          0xe6779f12, 0x00793ef1, 0xcee7ad28, 0xd03c31f4,
          0x3b549cb7, 0x7c65c13d, 0x1e7384fd, 0x71e45be8
};
int ref_18[1] = {
          0xc7d3e1fc
};
int ref_19[1] = {
          0xcee7ad28
};
int ref_20[2] = {
          0x3b549cb7, 0x7c65c13d
};
int ref_21[1] = {
          0xe81cc943
};
int ref_22[1] = {
          0x87802804
};
int ref_23[1] = {
          0x8659bea1
};
int out_24[1] = {
          0xe0c9b012
};
int ref_25[6] = {
          0x3515f08a, 0xe2ebcbaf, 0xf797d327, 0x87802804,
          0x438d8a74, 0x8659bea1
};
int out_26[6] = {
          0x66e081d4, 0x8462942f, 0xb2eaa762, 0xea1460c9,
          0xbead5c86, 0xddef4fd4
};
int ref_27[1] = {
          0xb25d2313
};
int ref_28[1] = {
          0xddef4fd4
};
int ref_29[1] = {
          0x25a59706
};
int out_30[36] = {
          0x5bdf951b, 0xfcb23817, 0xd6949a89, 0x3e514ed3,
          0x9932f7f3, 0x897e9031, 0x4b322075, 0xce38e455,
          0xd29ca772, 0x8ba08ace, 0xc851e321, 0x8932055e,
          0xbe09dab0, 0x88450fd4, 0x04338d40, 0xf71a0fec,
          0x8f8bdff5, 0x4677107d, 0x7ebb08e8, 0x8a6a1447,
          0xe56d48ea, 0x9f12727d, 0xdb7c9bcb, 0xa0bfd657,
          0x4c8bc729, 0xc60a0766, 0x80833cc2, 0xd8897ce5,
          0xd7419c67, 0xebe7882b, 0x090f4d09, 0x8d7e9a64,
          0x664446b5, 0xa92d2137, 0x37238a98, 0xfd49ffa2
};
int ref_31[28] = {
          0x5bdf951b, 0xfcb23817, 0xd6949a89, 0x3e514ed3,
          0x9932f7f3, 0x897e9031, 0x4b322075, 0xce38e455,
          0xd29ca772, 0x8ba08ace, 0xc851e321, 0x8932055e,
          0xbe09dab0, 0x88450fd4, 0x04338d40, 0xf71a0fec,
          0x8f8bdff5, 0x4677107d, 0x7ebb08e8, 0x8a6a1447,
          0xe56d48ea, 0x9f12727d, 0xdb7c9bcb, 0xa0bfd657,
          0x4c8bc729, 0xc60a0766, 0x80833cc2, 0xd8897ce5
};
int ref_32[9] = {
          0x4677107d, 0x7ebb08e8, 0x8a6a1447, 0xe56d48ea,
          0x9f12727d, 0xdb7c9bcb, 0xa0bfd657, 0x4c8bc729,
          0xc60a0766
};
int ref_33[1] = {
          0x8ba08ace
};
int ref_34[2] = {
          0x4677107d, 0x7ebb08e8
};
int ref_35[1] = {
          0x7ebb08e8
};
int ref_36[1] = {
          0x4677107d
};
int ref_37[1] = {
          0xc851e321
};
int ref_38[1] = {
          0xa0bfd657
};
int ref_39[1] = {
          0x8932055e
};
int ref_40[3] = {
          0xd29ca772, 0x8ba08ace, 0xc851e321
};
int ref_41[1] = {
          0xc60a0766
};
int ref_42[1] = {
          0xd29ca772
};
int ref_43[1] = {
          0x4c8bc729
};
int ref_44[1] = {
          0x9f12727d
};
int ref_45[2] = {
          0x4c8bc729, 0xc60a0766
};
int ref_46[13] = {
          0x4b322075, 0xce38e455, 0xd29ca772, 0x8ba08ace,
          0xc851e321, 0x8932055e, 0xbe09dab0, 0x88450fd4,
          0x04338d40, 0xf71a0fec, 0x8f8bdff5, 0x4677107d,
          0x7ebb08e8
};
int ref_47[1] = {
          0x8f8bdff5
};
int out_48[2] = {
          0x2de49f7b, 0xfcae6a92
};
int out_49[23] = {
          0x7caef3a6, 0x35946b5c, 0x90c1dee7, 0xb8b56ade,
          0xb5f34d31, 0xe466b145, 0x671fe4fd, 0xb97a7316,
          0x69a9a62a, 0x6e6c182f, 0xdc88fd4e, 0x53778dce,
          0xd83217e8, 0xaeead0a1, 0xb5a65ffc, 0xf18a3324,
          0x72d984b1, 0x716632b8, 0xba44c8e0, 0xc920baf9,
          0xe1805f2a, 0x4a92d7a6, 0x1072be76
};
int out_50[2] = {
          0xbe7c43a5, 0x33745902
};
int ref_51[2] = {
          0x8a6a1447, 0xe56d48ea
};
int ref_52[1] = {
          0x8a6a1447
};
int out_53[18] = {
          0xf0905c4c, 0xda9d7cb6, 0x54dfd1f6, 0xaff7c115,
          0x49370911, 0x31048e70, 0x7d0a4bf1, 0xa75137a1,
          0xe5a9f2fe, 0x2d1ffa0f, 0x6a155764, 0xbcc4f0b8,
          0xc04dc6f2, 0x3e17663f, 0x629c593d, 0x59462138,
          0xd57fafad, 0xc13c7180
};
int ref_54[3] = {
          0x8f8bdff5, 0x4677107d, 0x7ebb08e8
};
int out_55[2] = {
          0x8078bec9, 0x5ea1f03a
};
int out_56[1] = {
          0xee4b0ef3
};
int ref_57[1] = {
          0xe56d48ea
};
int ref_58[6] = {
          0xf0905c4c, 0xda9d7cb6, 0x54dfd1f6, 0xaff7c115,
          0x49370911, 0x31048e70
};
int ref_59[7] = {
          0x7d0a4bf1, 0xa75137a1, 0xe5a9f2fe, 0x2d1ffa0f,
          0x6a155764, 0xbcc4f0b8, 0xc04dc6f2
};
int ref_60[12] = {
          0xaff7c115, 0x49370911, 0x31048e70, 0x7d0a4bf1,
          0xa75137a1, 0xe5a9f2fe, 0x2d1ffa0f, 0x6a155764,
          0xbcc4f0b8, 0xc04dc6f2, 0x3e17663f, 0x629c593d
};
int ref_61[2] = {
          0x59462138, 0xd57fafad
};
int ref_62[1] = {
          0xc13c7180
};
int ref_63[1] = {
          0x629c593d
};
int ref_64[5] = {
          0xc851e321, 0x8932055e, 0xbe09dab0, 0x88450fd4,
          0x04338d40
};
int ref_65[1] = {
          0xbe09dab0
};
int ref_66[2] = {
          0xbe09dab0, 0x88450fd4
};
int ref_67[1] = {
          0x88450fd4
};
int ref_68[2] = {
          0xe56d48ea, 0x9f12727d
};
int out_69[1] = {
          0xe2397490
};
int out_70[3] = {
          0x99d5a91e, 0x9fb350cb, 0x2bec2c16
};
int out_71[2] = {
          0xf42c684d, 0xf27ed68e
};
int out_72[9] = {
          0xe95a18e6, 0x854474c2, 0xdc32039e, 0xff80a30f,
          0xe577591c, 0x49f2c4d2, 0xb0a701ad, 0x5c00acb0,
          0x1b9713a6
};
int ref_73[2] = {
          0xdb7c9bcb, 0xa0bfd657
};
int out_74[46] = {
          0x69af519a, 0x9ffc56a9, 0xa064a8e4, 0x3d8bb012,
          0xafa29eef, 0xc86fcd2f, 0x42527ffc, 0xc6ef097e,
          0x61071e7a, 0x6dbcea3e, 0xdeed6b4e, 0x5ce23443,
          0x5acca66a, 0x3ec29f7d, 0x9b534516, 0xd1ed4f39,
          0xb787e4e9, 0xdb2e1684, 0xeabfc23b, 0x267368e9,
          0xd8468f74, 0xb716fd36, 0x9a9e5d6f, 0x48a59a89,
          0x6cff13b0, 0x1e4dfdec, 0xab8d1734, 0x8b2aba35,
          0x73fbf1a3, 0x14e4d78f, 0xfee3ed26, 0x91f93718,
          0x506d1c79, 0x7f842bad, 0x1d22723c, 0x8c559018,
          0xa32ee8ff, 0x8c8fe2db, 0x4349b418, 0x1b8ea47f,
          0xf10d12b8, 0x90e2dd59, 0x31e196ce, 0x253c4c84,
          0x9f8bd982, 0x48219ca9
};
int out_75[48] = {
          0xa067bb92, 0xffd76962, 0x9b0d8895, 0xf09a54bf,
          0x4103fa47, 0x12ed55ed, 0x802bdca3, 0x37c6da72,
          0xe23ed88a, 0xb8066b88, 0xa61db17c, 0x008d74ec,
          0x84521b3e, 0x1e884893, 0x07ce3da0, 0x62a589e9,
          0xfd87c06c, 0x4b0a65ba, 0xab3b3aa1, 0x10952f52,
          0xb33b2ffe, 0x9518aa20, 0x12332a29, 0x1212af8f,
          0xcb8c9e91, 0x41d0ebbb, 0xf35a1148, 0x01bc7caa,
          0xd6c95ff1, 0x9c4ccfbd, 0xc732c75d, 0x18961e4e,
          0xf6980669, 0xd45efb56, 0x63b1071b, 0xea969421,
          0x0919aae0, 0xb5618e16, 0x55356f25, 0x27a58c5c,
          0x8be7671c, 0x785bdca8, 0xd2c154d9, 0xbde540ee,
          0x6606c2ac, 0xf11413a0, 0x02e36bf7, 0xa270dc43
};
int ref_76[1] = {
          0xdb7c9bcb
};
int out_77[1] = {
          0x8bfb13b7
};
int out_78[2] = {
          0x7c2715c1, 0xad0c1e54
};
int out_79[3] = {
          0x48052d10, 0x0bfd53d6, 0x44e48f23
};
int ref_80[6] = {
          0xa067bb92, 0xffd76962, 0x9b0d8895, 0xf09a54bf,
          0x4103fa47, 0x12ed55ed
};
int ref_81[24] = {
          0xc86fcd2f, 0x42527ffc, 0xc6ef097e, 0x61071e7a,
          0x6dbcea3e, 0xdeed6b4e, 0x5ce23443, 0x5acca66a,
          0x3ec29f7d, 0x9b534516, 0xd1ed4f39, 0xb787e4e9,
          0xdb2e1684, 0xeabfc23b, 0x267368e9, 0xd8468f74,
          0xb716fd36, 0x9a9e5d6f, 0x48a59a89, 0x6cff13b0,
          0x1e4dfdec, 0xab8d1734, 0x8b2aba35, 0x73fbf1a3
};
int ref_82[2] = {
          0x253c4c84, 0x9f8bd982
};
int out_83[2] = {
          0x904a809c, 0x7b488178
};
int out_84[24] = {
          0x5e21b058, 0xa993fc99, 0xfc54c5b5, 0x166c592b,
          0x7c06fa4d, 0x75530305, 0xe910395e, 0x77672c24,
          0xfd82cc6e, 0xdb64887d, 0xf46e5d34, 0xca047c39,
          0x9e53c575, 0x9a9b8800, 0xd3383e56, 0xd0ff986b,
          0xda73220f, 0x2ed33e1d, 0xfc7ade3f, 0x1ce01500,
          0x5d15b8e8, 0xbd30caf6, 0xc7899659, 0xa27c8e7c
};
int out_85[6] = {
          0x850eb076, 0x12b9bfa2, 0x2e9cd974, 0xe7add402,
          0xfb0b18c4, 0x2644a63d
};
int out_86[1] = {
          0xa33bb603
};
int ref_87[3] = {
          0x4349b418, 0x1b8ea47f, 0xf10d12b8
};
int out_88[3] = {
          0xb5b1b560, 0xc77184a6, 0x1b6745e8
};
int out_89[103] = {
          0x12b31a79, 0x5030318f, 0x8366e121, 0x06478ba0,
          0x14923508, 0x1404f71d, 0xa8735e64, 0x0fe72d8b,
          0xcc520f82, 0x76ed7a64, 0x7a4dcd4c, 0xb7736a6e,
          0xab4bce1f, 0x8740b778, 0x16efd962, 0xa1a2f850,
          0x587f1ec2, 0xbd2c5ba9, 0x5de8b549, 0x0e1cb63a,
          0x0166d1f6, 0x5a5d5f84, 0x178bcc28, 0xca3f3f4f,
          0x9adf2fb6, 0x1183641f, 0x94cb8d1c, 0x59193a45,
          0x7f1c2d2e, 0x270c8dad, 0xe236198b, 0x7e744039,
          0x3023e5cb, 0xc8e4af67, 0x6e903f47, 0x9921fd6d,
          0xe64a1138, 0x716c3a0b, 0x0aa7887c, 0xb2cb10b0,
          0x1d29211f, 0xff9e9d4a, 0x04c04e06, 0x36817794,
          0x4f97e67a, 0x896df156, 0x979d8e1e, 0xd17a7cb8,
          0x0254f60f, 0x615689a9, 0xe4c2c5a7, 0xbaf64059,
          0x45abba5a, 0x615ce2e9, 0x6049c692, 0x99bdff17,
          0xf7179cd1, 0xe92d54bc, 0x5e28fa36, 0xa00bfb84,
          0xcad4ae85, 0xf11575b1, 0x284bb7b1, 0x72542294,
          0x63607077, 0x751977c9, 0x78b2cf68, 0x5db0fd58,
          0x4ea2b5de, 0x9b70f3bd, 0xf3e4d26d, 0xa3d59a29,
          0x0609edd5, 0x2297fc77, 0xa2414396, 0xce696d6a,
          0x9e9d4e49, 0xfaa8084b, 0xbc865299, 0xc1f857c5,
          0x878124d5, 0x4180a45f, 0xd5112f8c, 0xf95f0f4d,
          0x0d6c46f0, 0x165cf242, 0x558ce6e5, 0x3c31d611,
          0x0c6f2ffb, 0x0c8ce646, 0xfdc13994, 0x43caa92b,
          0x9442d903, 0x4a60b526, 0x68565b31, 0xa0879995,
          0x295af10f, 0x5f487ee0, 0x260322c6, 0x3da6e326,
          0x96af01a8, 0xb9d0b261, 0x18f97f19
};
int out_90[1] = {
          0x02ff1b88
};
int out_91[1] = {
          0x19dc2024
};
int out_92[1] = {
          0xd9acb3ff
};
int out_93[29] = {
          0x6a6eaa23, 0x50f56665, 0x31e8b63a, 0x8f2a9be0,
          0xa24bfd75, 0xc0481f51, 0x11395d46, 0xc16546b3,
          0x582699fd, 0x0816a483, 0x0160d51b, 0x6982a2bd,
          0x4c8f0b38, 0x8833b42e, 0xe3c11367, 0x9783773f,
          0xfa4047ef, 0x3a66f278, 0xdb4b3f7d, 0x341401fa,
          0xcc7f7150, 0x122cca3c, 0xf1ed852a, 0x299dbde3,
          0x603a6673, 0x7c0d1d48, 0x292c4f82, 0x460509a9,
          0xfd7b3c8e
};
int out_94[1] = {
          0x2812d257
};
int out_95[7] = {
          0xd0c35424, 0x76e3c88b, 0x6c09a1b2, 0x7fa5bb4a,
          0x0ebeec78, 0xbd92e960, 0xdb7e9ccc
};
int out_96[1] = {
          0x2c3dae04
};
int out_97[1] = {
          0x646f7816
};
int out_98[1] = {
          0x8181c7fb
};
int out_99[2] = {
          0xf5bc89c8, 0xe3e6a0db
};
int out_100[3] = {
          0x3cf65883, 0xe19c1ce4, 0xd1f997ad
};
int ref_101[3] = {
          0x2c3dae04, 0xf5bc89c8, 0xe3e6a0db
};
int out_102[9] = {
          0xbd20f66e, 0xd905f8ca, 0x016cb29f, 0xd4616355,
          0x0381814c, 0x54d8eee1, 0x33ddf20f, 0x3777a397,
          0x51094de1
};
int ref_103[2] = {
          0x3777a397, 0x51094de1
};
int ref_104[1] = {
          0x51094de1
};
int ref_105[2] = {
          0xbd20f66e, 0xd905f8ca
};
int out_106[2] = {
          0x0abf904c, 0xdbd2d450
};
int out_107[5] = {
          0x893b67ab, 0x58c514fd, 0x08ba7512, 0x9eaa78ac,
          0x91fae0e6
};
int out_108[1] = {
          0xbfae9e47
};
int out_109[5] = {
          0x3396497b, 0xc82645ed, 0xbe1b805a, 0x21150291,
          0xdf5bf75e
};
int out_110[9] = {
          0x26bde805, 0xd81741df, 0x94e55832, 0x459e966f,
          0x494df85d, 0xe80895f3, 0x797aef29, 0x63ec9ef5,
          0x569db646
};
int out_111[1] = {
          0x3b2a8774
};
int out_112[1] = {
          0x51aaaecd
};
int out_113[3] = {
          0x9507c939, 0xde963905, 0xe0d6dc7b
};
int out_114[13] = {
          0x4763e9c1, 0x7c0b8203, 0x4f660158, 0xb7de516d,
          0x0375f1a3, 0x54002d0a, 0x5a3734ab, 0x97171e04,
          0x02c954eb, 0x78597978, 0xb8454298, 0x4a2c1e04,
          0x773ea1ec
};
int out_115[2] = {
          0xe6d4cf21, 0x96c9a681
};
int out_116[3] = {
          0x2210c716, 0xd04fe009, 0xd5c133d2
};
int out_117[1] = {
          0x0b121130
};
void func3(int *b)
{
    check(&b[915], ref_3, 2, "check_4 b", 915);
}
void func4(int *b)
{
    check(&b[917], ref_2, 1, "check_5 b", 917);
}
void func5(int *b)
{
}
void func6(int *b)
{
    check(&b[914], ref_1, 1, "check_6 b", 914);
    check(&b[917], ref_2, 1, "check_7 b", 917);
}
void func2(int *b)
{
    check(&b[914], ref_1, 1, "check_2 b", 914);
    check(&b[917], ref_2, 1, "check_3 b", 917);
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
    check(&b[914], ref_1, 1, "check_10 b", 914);
    check(&b[917], ref_2, 1, "check_11 b", 917);
}
void func9(int *b)
{
    check(&b[916], ref_5, 1, "check_12 b", 916);
    check(&b[914], ref_1, 1, "check_13 b", 914);
    check(&b[917], ref_2, 1, "check_14 b", 917);
}
void func10(int *b)
{
    check(&b[917], ref_2, 1, "check_15 b", 917);
}
void func11(int *b)
{
}
void func7(int *b)
{
    check(&b[916], ref_4, 2, "check_8 b", 916);
    check(&b[914], ref_1, 1, "check_9 b", 914);
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
    check(&b[914], ref_1, 1, "check_18 b", 914);
}
void func14(int *b)
{
    check(&b[917], ref_2, 1, "check_19 b", 917);
}
void func15(int *b)
{
    check(&b[917], ref_2, 1, "check_20 b", 917);
    check(&b[914], ref_1, 1, "check_21 b", 914);
}
void func16(int *b)
{
}
void func12(int *b)
{
    check(&b[917], ref_2, 1, "check_16 b", 917);
    check(&b[914], ref_1, 1, "check_17 b", 914);
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
    check(&b[916], ref_5, 1, "check_23 b", 916);
}
void func19(int *b)
{
    check(&b[917], ref_2, 1, "check_24 b", 917);
}
void func20(int *b)
{
}
void func21(int *b)
{
    check(&b[916], ref_5, 1, "check_25 b", 916);
}
void func17(int *b)
{
    check(&b[917], ref_2, 1, "check_22 b", 917);
    #pragma oss task in(b[916;1]) weakin(b[917;1]) node(3) label("func18")
    func18(b);

    #pragma oss task in(b[917;1]) weakin(b[916;1]) node(3) label("func19")
    func19(b);

    #pragma oss task weakin(b[917;1]) node(1) label("func20")
    func20(b);

    #pragma oss task in(b[916;1]) weakin(b[917;1]) node(2) label("func21")
    func21(b);

}
void func1(int *b)
{
    #pragma oss task in(b[914;1]) in(b[917;1]) weakin(b[915;2]) node(1) label("func2")
    func2(b);

    #pragma oss task in(b[916;2]) in(b[914;1]) node(3) label("func7")
    func7(b);

    #pragma oss task in(b[917;1]) in(b[914;1]) weakin(b[915;2]) node(3) label("func12")
    func12(b);

    #pragma oss task weakin(b[916;1]) in(b[917;1]) node(3) label("func17")
    func17(b);

}
void func24(int *b)
{
    check(&b[890], ref_9, 1, "check_29 b", 890);
    copy(&b[912], out_10, 1);
    copy(&b[911], out_11, 1);
}
void func25(int *b)
{
    check(&b[909], ref_12, 2, "check_30 b", 909);
    check(&b[893], ref_13, 2, "check_31 b", 893);
    copy(&b[912], out_14, 1);
}
void func26(int *b)
{
    check(&b[895], ref_15, 1, "check_32 b", 895);
}
void func27(int *b)
{
    check(&b[909], ref_16, 1, "check_33 b", 909);
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
    check(&b[913], ref_18, 1, "check_36 b", 913);
    check(&b[892], ref_19, 1, "check_37 b", 892);
}
void func30(int *b)
{
    check(&b[894], ref_20, 2, "check_38 b", 894);
}
void func31(int *b)
{
    check(&b[893], ref_13, 2, "check_39 b", 893);
}
void func32(int *b)
{
    check(&b[907], ref_21, 1, "check_40 b", 907);
}
void func28(int *b)
{
    check(&b[890], ref_17, 8, "check_34 b", 890);
    check(&b[913], ref_18, 1, "check_35 b", 913);
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
    check(&b[913], ref_18, 1, "check_41 b", 913);
}
void func35(int *b)
{
    check(&b[913], ref_18, 1, "check_42 b", 913);
}
void func36(int *b)
{
}
void func37(int *b)
{
    check(&b[913], ref_18, 1, "check_43 b", 913);
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
    check(&b[885], ref_22, 1, "check_44 b", 885);
}
void func40(int *b)
{
    check(&b[887], ref_23, 1, "check_45 b", 887);
}
void func41(int *b)
{
    check(&b[887], ref_23, 1, "check_46 b", 887);
    copy(&b[910], out_24, 1);
}
void func42(int *b)
{
    check(&b[882], ref_25, 6, "check_47 b", 882);
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
    check(&b[882], ref_8, 23, "check_28 b", 882);
    #pragma oss task out(b[911;2]) weakin(b[909;2]) weakin(b[889;11]) node(1) label("func23")
    func23(b);

    #pragma oss task in(b[890;8]) in(b[913;1]) weakin(b[907;1]) node(0) label("func28")
    func28(b);

    #pragma oss task weakin(b[913;1]) node(0) label("func33")
    func33(b);

    #pragma oss task weakin(b[882;6]) weakinout(b[910;1]) node(3) label("func38")
    func38(b);

}
void func45(int *b)
{
    check(&b[838], ref_27, 1, "check_50 b", 838);
}
void func46(int *b)
{
}
void func47(int *b)
{
    check(&b[838], ref_27, 1, "check_51 b", 838);
}
void func48(int *b)
{
    check(&b[838], ref_27, 1, "check_52 b", 838);
}
void func44(int *b)
{
    check(&b[838], ref_27, 1, "check_49 b", 838);
    #pragma oss task in(b[838;1]) node(0) label("func45")
    func45(b);

    #pragma oss task weakin(b[838;1]) node(1) label("func46")
    func46(b);

    #pragma oss task in(b[838;1]) node(3) label("func47")
    func47(b);

    #pragma oss task in(b[838;1]) node(0) label("func48")
    func48(b);

}
void func50(int *b)
{
}
void func51(int *b)
{
    check(&b[923], ref_28, 1, "check_54 b", 923);
}
void func52(int *b)
{
    check(&b[923], ref_28, 1, "check_55 b", 923);
}
void func53(int *b)
{
}
void func49(int *b)
{
    check(&b[923], ref_28, 1, "check_53 b", 923);
    #pragma oss task weakin(b[923;1]) node(3) label("func50")
    func50(b);

    #pragma oss task in(b[923;1]) node(1) label("func51")
    func51(b);

    #pragma oss task in(b[923;1]) node(1) label("func52")
    func52(b);

    #pragma oss task weakin(b[923;1]) node(3) label("func53")
    func53(b);

}
void func55(int *b)
{
    check(&b[923], ref_28, 1, "check_58 b", 923);
}
void func56(int *b)
{
}
void func57(int *b)
{
    check(&b[923], ref_28, 1, "check_59 b", 923);
    check(&b[838], ref_27, 1, "check_60 b", 838);
}
void func58(int *b)
{
}
void func54(int *b)
{
    check(&b[838], ref_27, 1, "check_56 b", 838);
    check(&b[923], ref_28, 1, "check_57 b", 923);
    #pragma oss task weakin(b[838;1]) in(b[923;1]) node(1) label("func55")
    func55(b);

    #pragma oss task weakin(b[923;1]) weakin(b[838;1]) node(1) label("func56")
    func56(b);

    #pragma oss task in(b[923;1]) in(b[838;1]) node(2) label("func57")
    func57(b);

    #pragma oss task weakin(b[923;1]) weakin(b[838;1]) node(1) label("func58")
    func58(b);

}
void func60(int *b)
{
    check(&b[923], ref_28, 1, "check_62 b", 923);
    check(&b[838], ref_27, 1, "check_63 b", 838);
}
void func61(int *b)
{
    check(&b[923], ref_28, 1, "check_64 b", 923);
}
void func62(int *b)
{
    check(&b[838], ref_27, 1, "check_65 b", 838);
}
void func63(int *b)
{
}
void func59(int *b)
{
    check(&b[923], ref_28, 1, "check_61 b", 923);
    #pragma oss task in(b[923;1]) in(b[838;1]) node(2) label("func60")
    func60(b);

    #pragma oss task in(b[923;1]) weakin(b[838;1]) node(3) label("func61")
    func61(b);

    #pragma oss task weakin(b[923;1]) in(b[838;1]) node(3) label("func62")
    func62(b);

    #pragma oss task weakin(b[838;1]) weakin(b[923;1]) node(3) label("func63")
    func63(b);

}
void func43(int *b)
{
    #pragma oss task in(b[838;1]) node(2) label("func44")
    func44(b);

    #pragma oss task in(b[923;1]) node(0) label("func49")
    func49(b);

    #pragma oss task in(b[838;1]) in(b[923;1]) node(1) label("func54")
    func54(b);

    #pragma oss task weakin(b[838;1]) in(b[923;1]) node(2) label("func59")
    func59(b);

}
void func66(int *b)
{
}
void func67(int *b)
{
    check(&b[837], ref_29, 1, "check_68 b", 837);
    check(&b[838], ref_27, 1, "check_69 b", 838);
}
void func68(int *b)
{
    check(&b[838], ref_27, 1, "check_70 b", 838);
}
void func69(int *b)
{
}
void func65(int *b)
{
    check(&b[837], ref_29, 1, "check_66 b", 837);
    check(&b[838], ref_27, 1, "check_67 b", 838);
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
    check(&b[838], ref_27, 1, "check_72 b", 838);
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
    check(&b[837], out_6, 2, "check_71 b", 837);
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
    check(&b[838], ref_27, 1, "check_74 b", 838);
}
void func77(int *b)
{
    check(&b[838], ref_27, 1, "check_75 b", 838);
}
void func78(int *b)
{
}
void func79(int *b)
{
}
void func75(int *b)
{
    check(&b[837], ref_29, 1, "check_73 b", 837);
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
    check(&b[838], ref_27, 1, "check_77 b", 838);
}
void func83(int *b)
{
    check(&b[838], ref_27, 1, "check_78 b", 838);
}
void func84(int *b)
{
    check(&b[838], ref_27, 1, "check_79 b", 838);
}
void func80(int *b)
{
    check(&b[838], ref_27, 1, "check_76 b", 838);
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
    copy(&b[914], out_0, 4);
    #pragma oss task weakin(b[914;4]) node(0) label("func1")
    func1(b);

    copy(&b[837], out_6, 2);
    copy(&b[882], out_7, 32);
    #pragma oss task weakout(b[833;1]) in(b[882;23]) weakinout(b[905;11]) node(0) label("func22")
    func22(b);

    copy(&b[918], out_26, 6);
    #pragma oss task weakin(b[837;2]) weakin(b[923;1]) weakout(b[887;21]) weakout(b[908;11]) node(2) label("func43")
    func43(b);

    #pragma oss task weakin(b[837;2]) node(3) label("func64")
    func64(b);

	printf("%s\n", __func__);

}
void func88(int *b)
{
    check(&b[852], ref_33, 1, "check_85 b", 852);
}
void func89(int *b)
{
    check(&b[861], ref_35, 1, "check_86 b", 861);
    check(&b[860], ref_36, 1, "check_87 b", 860);
}
void func90(int *b)
{
    check(&b[853], ref_37, 1, "check_88 b", 853);
}
void func91(int *b)
{
    check(&b[860], ref_36, 1, "check_89 b", 860);
}
void func87(int *b)
{
    check(&b[852], ref_33, 1, "check_83 b", 852);
    check(&b[860], ref_34, 2, "check_84 b", 860);
    #pragma oss task in(b[852;1]) weakin(b[853;1]) node(0) label("func88")
    func88(b);

    #pragma oss task in(b[861;1]) weakin(b[853;1]) weakin(b[852;1]) in(b[860;1]) node(1) label("func89")
    func89(b);

    #pragma oss task in(b[853;1]) weakin(b[852;1]) node(3) label("func90")
    func90(b);

    #pragma oss task weakin(b[852;1]) in(b[860;1]) weakin(b[853;1]) node(0) label("func91")
    func91(b);

}
void func93(int *b)
{
    check(&b[854], ref_39, 1, "check_91 b", 854);
    check(&b[866], ref_38, 1, "check_92 b", 866);
}
void func94(int *b)
{
    check(&b[866], ref_38, 1, "check_93 b", 866);
}
void func95(int *b)
{
    check(&b[854], ref_39, 1, "check_94 b", 854);
    check(&b[866], ref_38, 1, "check_95 b", 866);
}
void func96(int *b)
{
    check(&b[854], ref_39, 1, "check_96 b", 854);
}
void func92(int *b)
{
    check(&b[866], ref_38, 1, "check_90 b", 866);
    #pragma oss task in(b[854;1]) in(b[866;1]) node(2) label("func93")
    func93(b);

    #pragma oss task weakin(b[854;1]) in(b[866;1]) node(2) label("func94")
    func94(b);

    #pragma oss task in(b[854;1]) in(b[866;1]) node(3) label("func95")
    func95(b);

    #pragma oss task weakin(b[866;1]) in(b[854;1]) node(2) label("func96")
    func96(b);

}
void func98(int *b)
{
    check(&b[868], ref_41, 1, "check_98 b", 868);
    check(&b[851], ref_42, 1, "check_99 b", 851);
}
void func99(int *b)
{
    check(&b[853], ref_37, 1, "check_100 b", 853);
    check(&b[867], ref_43, 1, "check_101 b", 867);
}
void func100(int *b)
{
    check(&b[867], ref_43, 1, "check_102 b", 867);
    check(&b[852], ref_33, 1, "check_103 b", 852);
}
void func101(int *b)
{
}
void func97(int *b)
{
    check(&b[851], ref_40, 3, "check_97 b", 851);
    #pragma oss task in(b[868;1]) in(b[851;1]) node(0) label("func98")
    func98(b);

    #pragma oss task in(b[853;1]) in(b[867;1]) node(0) label("func99")
    func99(b);

    #pragma oss task weakin(b[868;1]) weakin(b[851;1]) in(b[867;1]) in(b[852;1]) node(1) label("func100")
    func100(b);

    #pragma oss task weakin(b[867;1]) weakin(b[868;1]) node(1) label("func101")
    func101(b);

}
void func103(int *b)
{
    check(&b[864], ref_44, 1, "check_107 b", 864);
}
void func104(int *b)
{
    check(&b[867], ref_45, 2, "check_108 b", 867);
    check(&b[853], ref_37, 1, "check_109 b", 853);
}
void func105(int *b)
{
}
void func106(int *b)
{
    check(&b[853], ref_37, 1, "check_110 b", 853);
}
void func102(int *b)
{
    check(&b[853], ref_37, 1, "check_104 b", 853);
    check(&b[852], ref_33, 1, "check_105 b", 852);
    check(&b[864], ref_44, 1, "check_106 b", 864);
    #pragma oss task in(b[864;1]) weakin(b[853;1]) weakin(b[868;1]) node(1) label("func103")
    func103(b);

    #pragma oss task in(b[867;2]) in(b[853;1]) node(1) label("func104")
    func104(b);

    #pragma oss task weakin(b[852;1]) weakin(b[864;1]) weakin(b[853;1]) node(0) label("func105")
    func105(b);

    #pragma oss task weakin(b[868;1]) weakin(b[852;1]) weakin(b[867;1]) in(b[853;1]) node(1) label("func106")
    func106(b);

}
void func86(int *b)
{
    check(&b[860], ref_32, 9, "check_82 b", 860);
    #pragma oss task in(b[852;1]) in(b[860;2]) weakin(b[853;1]) node(2) label("func87")
    func87(b);

    #pragma oss task weakin(b[854;1]) in(b[866;1]) node(0) label("func92")
    func92(b);

    #pragma oss task in(b[851;3]) weakin(b[867;1]) weakin(b[868;1]) node(1) label("func97")
    func97(b);

    #pragma oss task in(b[853;1]) weakin(b[867;2]) in(b[852;1]) in(b[864;1]) node(0) label("func102")
    func102(b);

}
void func109(int *a,int *b)
{
    check(&b[859], ref_47, 1, "check_112 b", 859);
}
void func110(int *a,int *b)
{
}
void func111(int *a,int *b)
{
}
void func112(int *a,int *b)
{
    copy(&a[707], out_49, 23);
    copy(&a[730], out_50, 2);
}
void func108(int *a,int *b)
{
    #pragma oss task weakout(a[725;4]) in(b[859;1]) weakin(b[860;1]) node(2) label("func109")
    func109(a,b);

    #pragma oss task weakin(b[859;2]) weakout(a[708;19]) node(3) label("func110")
    func110(a,b);

    #pragma oss task weakin(b[860;1]) weakout(a[707;17]) node(1) label("func111")
    func111(a,b);

    copy(&a[730], out_48, 2);
    #pragma oss task out(a[707;23]) out(a[730;2]) weakin(b[859;2]) node(3) label("func112")
    func112(a,b);

}
void func114(int *a,int *b)
{
}
void func115(int *b)
{
    check(&b[862], ref_51, 2, "check_114 b", 862);
}
void func116(int *a,int *b)
{
    check(&b[862], ref_52, 1, "check_115 b", 862);
}
void func117(int *a,int *b)
{
}
void func113(int *a,int *b)
{
    #pragma oss task weakin(b[863;1]) weakout(a[768;1]) node(0) label("func114")
    func114(a,b);

    #pragma oss task in(b[862;2]) node(0) label("func115")
    func115(b);

    #pragma oss task weakout(a[748;10]) in(b[862;1]) node(2) label("func116")
    func116(a,b);

    #pragma oss task weakin(b[862;2]) weakout(a[758;3]) node(2) label("func117")
    func117(a,b);

}
void func119(int *a,int *b)
{
    check(&b[862], ref_52, 1, "check_120 b", 862);
}
void func120(int *a,int *b)
{
    check(&a[752], out_55, 2, "check_121 a", 752);
}
void func121(int *a,int *b)
{
    check(&b[859], ref_54, 3, "check_122 b", 859);
    copy(&a[722], out_56, 1);
}
void func122(int *a,int *b)
{
}
void func118(int *a,int *b)
{
    check(&b[859], ref_54, 3, "check_117 b", 859);
    check(&b[862], ref_52, 1, "check_118 b", 862);
    copy(&a[752], out_55, 2);
    #pragma oss task in(b[862;1]) weakin(b[859;3]) weakinout(a[752;1]) node(0) label("func119")
    func119(a,b);

    #pragma oss task weakin(b[862;1]) weakin(b[860;2]) in(a[752;2]) node(0) label("func120")
    func120(a,b);

    #pragma oss task out(a[722;1]) in(b[859;3]) node(0) label("func121")
    func121(a,b);

    #pragma oss task weakin(a[752;2]) weakin(b[862;1]) weakout(a[754;5]) node(3) label("func122")
    func122(a,b);

}
void func124(int *a,int *b)
{
    check(&b[863], ref_57, 1, "check_124 b", 863);
    check(&a[457], ref_58, 6, "check_125 a", 457);
    check(&a[463], ref_59, 7, "check_126 a", 463);
}
void func125(int *a)
{
}
void func126(int *a)
{
    check(&a[460], ref_60, 12, "check_127 a", 460);
    check(&a[472], ref_61, 2, "check_128 a", 472);
}
void func127(int *a,int *b)
{
    check(&a[474], ref_62, 1, "check_129 a", 474);
    check(&b[863], ref_57, 1, "check_130 b", 863);
    check(&a[471], ref_63, 1, "check_131 a", 471);
}
void func123(int *a,int *b)
{
    check(&a[457], out_53, 18, "check_123 a", 457);
    #pragma oss task in(b[863;1]) weakin(a[470;5]) in(a[457;6]) in(a[463;7]) node(1) label("func124")
    func124(a,b);

    #pragma oss task weakin(a[471;1]) weakin(a[472;1]) weakin(a[473;1]) node(0) label("func125")
    func125(a);

    #pragma oss task in(a[460;12]) in(a[472;2]) weakin(a[474;1]) node(1) label("func126")
    func126(a);

    #pragma oss task in(a[474;1]) in(b[863;1]) in(a[471;1]) node(2) label("func127")
    func127(a,b);

}
void func107(int *a,int *b)
{
    check(&b[849], ref_46, 13, "check_111 b", 849);
    #pragma oss task weakin(b[859;2]) out(a[706;26]) node(0) label("func108")
    func108(a,b);

    #pragma oss task weakin(b[862;2]) weakout(a[723;48]) node(1) label("func113")
    func113(a,b);

    copy(&a[457], out_53, 18);
    #pragma oss task out(a[705;26]) out(a[731;40]) in(b[859;3]) in(b[862;1]) node(2) label("func118")
    func118(a,b);

    #pragma oss task weakin(b[863;1]) in(a[457;18]) node(0) label("func123")
    func123(a,b);

}
void func130(int *b)
{
    check(&b[863], ref_57, 1, "check_136 b", 863);
    check(&b[855], ref_65, 1, "check_137 b", 855);
}
void func131(int *b)
{
    check(&b[863], ref_57, 1, "check_138 b", 863);
    check(&b[854], ref_39, 1, "check_139 b", 854);
    check(&b[855], ref_65, 1, "check_140 b", 855);
}
void func132(int *b)
{
}
void func133(int *b)
{
    check(&b[854], ref_39, 1, "check_141 b", 854);
}
void func129(int *b)
{
    check(&b[863], ref_57, 1, "check_133 b", 863);
    check(&b[855], ref_65, 1, "check_134 b", 855);
    check(&b[854], ref_39, 1, "check_135 b", 854);
    #pragma oss task in(b[863;1]) in(b[855;1]) node(1) label("func130")
    func130(b);

    #pragma oss task in(b[863;1]) in(b[854;1]) in(b[855;1]) node(1) label("func131")
    func131(b);

    #pragma oss task weakin(b[855;1]) weakin(b[854;1]) weakin(b[863;1]) node(0) label("func132")
    func132(b);

    #pragma oss task weakin(b[855;1]) weakin(b[863;1]) in(b[854;1]) node(0) label("func133")
    func133(b);

}
void func135(int *b)
{
    check(&b[856], ref_67, 1, "check_144 b", 856);
}
void func136(int *b)
{
    check(&b[856], ref_67, 1, "check_145 b", 856);
}
void func137(int *b)
{
    check(&b[864], ref_44, 1, "check_146 b", 864);
    check(&b[856], ref_67, 1, "check_147 b", 856);
}
void func138(int *b)
{
    check(&b[864], ref_44, 1, "check_148 b", 864);
}
void func134(int *b)
{
    check(&b[855], ref_66, 2, "check_142 b", 855);
    check(&b[864], ref_44, 1, "check_143 b", 864);
    #pragma oss task weakin(b[855;1]) weakin(b[864;1]) in(b[856;1]) node(2) label("func135")
    func135(b);

    #pragma oss task in(b[856;1]) weakin(b[864;1]) node(3) label("func136")
    func136(b);

    #pragma oss task in(b[864;1]) in(b[856;1]) node(2) label("func137")
    func137(b);

    #pragma oss task in(b[864;1]) node(1) label("func138")
    func138(b);

}
void func140(int *b)
{
}
void func141(int *b)
{
    check(&b[864], ref_44, 1, "check_152 b", 864);
    check(&b[861], ref_35, 1, "check_153 b", 861);
}
void func142(int *b)
{
}
void func143(int *b)
{
    check(&b[861], ref_35, 1, "check_154 b", 861);
}
void func139(int *b)
{
    check(&b[856], ref_67, 1, "check_149 b", 856);
    check(&b[863], ref_68, 2, "check_150 b", 863);
    check(&b[854], ref_39, 1, "check_151 b", 854);
    #pragma oss task weakin(b[861;1]) weakin(b[856;1]) weakin(b[854;1]) node(2) label("func140")
    func140(b);

    #pragma oss task weakin(b[856;1]) in(b[864;1]) in(b[861;1]) node(3) label("func141")
    func141(b);

    #pragma oss task weakin(b[864;1]) weakin(b[856;1]) weakin(b[854;1]) node(1) label("func142")
    func142(b);

    #pragma oss task in(b[861;1]) weakin(b[854;1]) node(2) label("func143")
    func143(b);

}
void func145(int *a,int *b)
{
}
void func146(int *a,int *b)
{
    check(&b[853], ref_37, 1, "check_158 b", 853);
}
void func147(int *a,int *b)
{
    check(&b[853], ref_37, 1, "check_160 b", 853);
    copy(&a[647], out_72, 9);
}
void func148(int *a,int *b)
{
}
void func144(int *a,int *b)
{
    check(&b[853], ref_37, 1, "check_155 b", 853);
    copy(&a[665], out_69, 1);
    #pragma oss task weakin(b[855;1]) weakout(a[650;13]) node(0) label("func145")
    func145(a,b);

    copy(&a[645], out_70, 3);
    #pragma oss task in(b[853;1]) weakin(a[665;1]) node(1) label("func146")
    func146(a,b);

    copy(&a[663], out_71, 2);
    #pragma oss task out(a[647;9]) in(b[853;1]) node(2) label("func147")
    func147(a,b);

    #pragma oss task weakin(a[645;2]) weakin(b[853;1]) node(1) label("func148")
    func148(a,b);

}
void func128(int *a,int *b)
{
    check(&b[853], ref_64, 5, "check_132 b", 853);
    #pragma oss task in(b[863;1]) in(b[855;1]) in(b[854;1]) node(1) label("func129")
    func129(b);

    #pragma oss task in(b[855;2]) in(b[864;1]) node(2) label("func134")
    func134(b);

    #pragma oss task weakin(b[861;1]) in(b[856;1]) in(b[863;2]) in(b[854;1]) node(2) label("func139")
    func139(b);

    #pragma oss task out(a[645;24]) in(b[853;1]) weakin(b[854;2]) node(1) label("func144")
    func144(a,b);

}
void func151(int *b)
{
    check(&b[866], ref_38, 1, "check_164 b", 866);
    check(&b[865], ref_76, 1, "check_165 b", 865);
}
void func152(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_166 b", 866);
    check(&b[865], ref_76, 1, "check_167 b", 865);
    copy(&a[755], out_77, 1);
}
void func153(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_168 b", 866);
    check(&b[865], ref_76, 1, "check_169 b", 865);
    copy(&a[754], out_78, 2);
}
void func154(int *b)
{
    check(&b[866], ref_38, 1, "check_170 b", 866);
}
void func150(int *a,int *b)
{
    #pragma oss task in(b[866;1]) in(b[865;1]) node(1) label("func151")
    func151(b);

    #pragma oss task out(a[755;1]) in(b[866;1]) in(b[865;1]) node(3) label("func152")
    func152(a,b);

    #pragma oss task in(b[866;1]) out(a[754;2]) in(b[865;1]) node(0) label("func153")
    func153(a,b);

    #pragma oss task weakin(b[865;1]) in(b[866;1]) node(3) label("func154")
    func154(b);

}
void func156(int *b)
{
}
void func157(int *a,int *b)
{
    copy(&a[743], out_79, 3);
}
void func158(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_172 b", 866);
}
void func159(int *a,int *b)
{
}
void func155(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_171 b", 866);
    #pragma oss task weakin(b[866;1]) node(1) label("func156")
    func156(b);

    #pragma oss task out(a[743;3]) weakin(b[866;1]) node(1) label("func157")
    func157(a,b);

    #pragma oss task in(b[866;1]) weakout(a[744;3]) node(2) label("func158")
    func158(a,b);

    #pragma oss task weakout(a[743;3]) weakin(b[866;1]) node(0) label("func159")
    func159(a,b);

}
void func161(int *b)
{
    check(&b[866], ref_38, 1, "check_174 b", 866);
}
void func162(int *b)
{
}
void func163(int *b)
{
}
void func164(int *b)
{
    check(&b[866], ref_38, 1, "check_175 b", 866);
}
void func160(int *b)
{
    check(&b[866], ref_38, 1, "check_173 b", 866);
    #pragma oss task in(b[866;1]) node(3) label("func161")
    func161(b);

    #pragma oss task weakin(b[866;1]) node(1) label("func162")
    func162(b);

    #pragma oss task weakin(b[866;1]) node(0) label("func163")
    func163(b);

    #pragma oss task in(b[866;1]) node(2) label("func164")
    func164(b);

}
void func166(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_178 b", 866);
}
void func167(int *a)
{
    check(&a[534], ref_81, 24, "check_179 a", 534);
    check(&a[572], ref_82, 2, "check_180 a", 572);
    copy(&a[532], out_83, 2);
    copy(&a[534], out_84, 24);
}
void func168(int *a,int *b)
{
    check(&b[866], ref_38, 1, "check_181 b", 866);
    check(&a[711], ref_80, 6, "check_182 a", 711);
    copy(&a[711], out_85, 6);
    copy(&a[574], out_86, 1);
}
void func169(int *a,int *b)
{
    check(&a[567], ref_87, 3, "check_183 a", 567);
    copy(&a[567], out_88, 3);
}
void func165(int *a,int *b)
{
    check(&a[711], ref_80, 6, "check_176 a", 711);
    check(&b[866], ref_38, 1, "check_177 b", 866);
    #pragma oss task weakin(a[715;2]) weakinout(a[545;25]) in(b[866;1]) node(1) label("func166")
    func166(a,b);

    #pragma oss task out(a[532;2]) inout(a[534;24]) in(a[572;2]) weakin(a[711;3]) node(3) label("func167")
    func167(a);

    #pragma oss task in(b[866;1]) inout(a[711;6]) out(a[574;1]) node(3) label("func168")
    func168(a,b);

    #pragma oss task inout(a[567;3]) weakin(b[866;1]) weakout(a[711;4]) weakout(a[715;2]) node(0) label("func169")
    func169(a,b);

}
void func149(int *a,int *b)
{
    check(&b[865], ref_73, 2, "check_161 b", 865);
    copy(&a[529], out_74, 46);
    copy(&a[711], out_75, 48);
    #pragma oss task weakin(b[865;1]) weakin(b[866;1]) out(a[752;4]) node(1) label("func150")
    func150(a,b);

    #pragma oss task in(b[866;1]) weakout(a[743;4]) node(3) label("func155")
    func155(a,b);

    #pragma oss task in(b[866;1]) node(2) label("func160")
    func160(b);

    #pragma oss task inout(a[711;6]) in(b[866;1]) weakinout(a[529;46]) node(0) label("func165")
    func165(a,b);

}
void func85(int *a,int *b)
{
    check(&b[843], ref_31, 28, "check_81 b", 843);
    #pragma oss task weakin(b[851;4]) in(b[860;9]) node(2) label("func86")
    func86(b);

    #pragma oss task weakout(a[686;87]) in(b[849;13]) weakin(b[862;2]) out(a[302;209]) node(0) label("func107")
    func107(a,b);

    #pragma oss task weakout(a[384;300]) in(b[853;5]) weakin(b[858;7]) node(3) label("func128")
    func128(a,b);

    #pragma oss task in(b[865;2]) out(a[465;303]) node(1) label("func149")
    func149(a,b);

	printf("%s\n", __func__);

}
void func173(int *b)
{
    copy(&b[761], out_92, 1);
}
void func174(int *b)
{
    copy(&b[686], out_93, 29);
}
void func175(int *b)
{
}
void func176(int *b)
{
    copy(&b[762], out_94, 1);
}
void func172(int *b)
{
    copy(&b[762], out_90, 1);
    copy(&b[761], out_91, 1);
    #pragma oss task weakout(b[762;1]) out(b[761;1]) weakout(b[699;37]) node(0) label("func173")
    func173(b);

    #pragma oss task out(b[686;29]) weakout(b[761;1]) node(1) label("func174")
    func174(b);

    #pragma oss task weakout(b[762;1]) node(2) label("func175")
    func175(b);

    #pragma oss task out(b[762;1]) node(0) label("func176")
    func176(b);

}
void func178(int *b)
{
    copy(&b[769], out_95, 7);
}
void func179(int *b)
{
    copy(&b[773], out_100, 3);
}
void func180(int *b)
{
    check(&b[776], out_97, 1, "check_193 b", 776);
}
void func181(int *b)
{
    check(&b[766], ref_101, 3, "check_196 b", 766);
}
void func177(int *b)
{
    #pragma oss task out(b[769;7]) node(0) label("func178")
    func178(b);

    copy(&b[766], out_96, 1);
    copy(&b[776], out_97, 1);
    copy(&b[765], out_98, 1);
    copy(&b[767], out_99, 2);
    #pragma oss task out(b[773;3]) weakinout(b[768;1]) weakin(b[776;1]) node(2) label("func179")
    func179(b);

    #pragma oss task in(b[776;1]) weakout(b[769;7]) node(0) label("func180")
    func180(b);

    #pragma oss task weakin(b[776;1]) weakout(b[775;1]) in(b[766;3]) node(0) label("func181")
    func181(b);

}
void func183(int *b)
{
    check(&b[786], ref_103, 2, "check_198 b", 786);
}
void func184(int *b)
{
    check(&b[787], ref_104, 1, "check_199 b", 787);
}
void func185(int *b)
{
    check(&b[779], ref_105, 2, "check_200 b", 779);
    check(&b[786], ref_103, 2, "check_201 b", 786);
    copy(&b[786], out_106, 2);
}
void func186(int *b)
{
    check(&b[786], out_106, 2, "check_202 b", 786);
    check(&b[779], ref_105, 2, "check_203 b", 779);
}
void func182(int *b)
{
    copy(&b[779], out_102, 9);
    #pragma oss task weakout(b[795;3]) weakout(b[781;5]) in(b[786;2]) node(1) label("func183")
    func183(b);

    #pragma oss task in(b[787;1]) weakin(b[779;2]) weakout(b[794;1]) node(0) label("func184")
    func184(b);

    #pragma oss task in(b[779;2]) inout(b[786;2]) node(3) label("func185")
    func185(b);

    #pragma oss task in(b[786;2]) in(b[779;2]) weakout(b[791;9]) node(1) label("func186")
    func186(b);

}
void func188(int *b)
{
    check(&b[764], out_107, 5, "check_205 b", 764);
    copy(&b[776], out_108, 1);
}
void func189(int *b)
{
    check(&b[764], out_107, 5, "check_206 b", 764);
    copy(&b[752], out_109, 5);
    copy(&b[769], out_110, 9);
}
void func190(int *b)
{
    check(&b[764], out_107, 5, "check_207 b", 764);
}
void func191(int *b)
{
}
void func187(int *b)
{
    copy(&b[764], out_107, 5);
    #pragma oss task out(b[776;1]) in(b[764;5]) node(3) label("func188")
    func188(b);

    #pragma oss task out(b[752;5]) in(b[764;5]) out(b[769;9]) node(1) label("func189")
    func189(b);

    #pragma oss task in(b[764;5]) node(2) label("func190")
    func190(b);

    #pragma oss task weakinout(b[764;1]) weakout(b[753;8]) weakinout(b[765;4]) node(3) label("func191")
    func191(b);

}
void func171(int *b)
{
    #pragma oss task weakout(b[680;62]) out(b[761;2]) node(3) label("func172")
    func172(b);

    #pragma oss task out(b[765;12]) node(0) label("func177")
    func177(b);

    #pragma oss task out(b[769;31]) node(1) label("func182")
    func182(b);

    #pragma oss task out(b[752;12]) out(b[764;14]) node(3) label("func187")
    func187(b);

}
void func194(int *a)
{
}
void func195(int *a)
{
    check(&a[529], out_111, 1, "check_209 a", 529);
    copy(&a[529], out_112, 1);
}
void func196(int *a)
{
}
void func197(int *a)
{
}
void func193(int *a)
{
    copy(&a[529], out_111, 1);
    #pragma oss task weakinout(a[529;1]) node(0) label("func194")
    func194(a);

    #pragma oss task inout(a[529;1]) weakout(a[517;4]) node(0) label("func195")
    func195(a);

    copy(&a[524], out_113, 3);
    #pragma oss task weakinout(a[529;1]) weakin(a[524;3]) node(3) label("func196")
    func196(a);

    copy(&a[502], out_114, 13);
    copy(&a[521], out_115, 2);
    #pragma oss task weakin(a[529;1]) weakout(a[520;2]) node(3) label("func197")
    func197(a);

}
void func199(int *a)
{
    copy(&a[528], out_116, 3);
    copy(&a[531], out_117, 1);
}
void func198(int *a)
{
    #pragma oss task out(a[528;3]) out(a[531;1]) node(3) label("func199")
    func199(a);

}
void func192(int *a)
{
    #pragma oss task out(a[529;1]) out(a[502;26]) node(3) label("func193")
    func193(a);

    #pragma oss task out(a[521;11]) weakout(a[509;2]) node(3) label("func198")
    func198(a);

}
void func170(int *a,int *b)
{
    #pragma oss task weakout(b[672;133]) node(3) label("func171")
    func171(b);

    #pragma oss task weakout(a[460;75]) node(0) label("func192")
    func192(a);

	printf("%s\n", __func__);
}

bool fail10()
{
    int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(1000 * sizeof(int));


    #pragma oss task out(b[879;54]) out(b[820;19]) node(0) label("func0")
    func0(b);

    copy(&b[843], out_30, 36);
    #pragma oss task weakout(a[269;508]) in(b[843;28]) node(2) label("func85")
    func85(a,b);

    copy(&b[294], out_89, 103);
    #pragma oss task weakout(a[379;254]) out(b[660;178]) weakin(b[843;21]) node(2) label("func170")
    func170(a,b);

	printf("Before taskwait\n");

    #pragma oss taskwait

	printf("After taskwait\n");
    return true;
}

int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail10);

	return run_test(&test);
}
