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


int out_0[416] = {
          0xcaf7a1a8, 0xe203ce02, 0x726ed005, 0x8616ca50, 
          0xa35ebe2c, 0x1c9d10d8, 0xd96ffddb, 0x1ba1310a, 
          0x1e2f64ee, 0xef39ff26, 0xb07d0116, 0x86b90ec1, 
          0xb2acc70b, 0x47a9420f, 0x6134b13c, 0x599a9ca3, 
          0xf8c9a98d, 0x4bb41ff1, 0x4f3951d3, 0xa0d6de53, 
          0x7187f64c, 0xa30bea1c, 0x6bf4183a, 0x035f6841, 
          0x8e363c64, 0x0e0986d1, 0x12978529, 0xca041854, 
          0x1eb44cd1, 0x2f64b101, 0x98abba27, 0x77f34c9d, 
          0x049ca8b8, 0x32f65cb8, 0x0b09235e, 0x6e6ede9f, 
          0x6fc07d7a, 0x5dfb50c9, 0x01588a98, 0x1abc1a6b, 
          0xa992a9e5, 0x725ec4fa, 0x09693ea8, 0x7678acc5, 
          0xdbde340d, 0x95da4662, 0x7e337745, 0xbbb72578, 
          0x55079543, 0xfb9312b2, 0x7fa73866, 0x3d6cebfd, 
          0xcdc09aad, 0x3103459c, 0x196501a9, 0x761c7882, 
          0x4985bbae, 0x0ae3f61d, 0x5a17bbba, 0x42a0af70, 
          0x65358eb9, 0xf0acc4c1, 0x99699480, 0x826e3fd1, 
          0xf210cfd9, 0xb277a5ca, 0xd8435d2d, 0x5235fa9c, 
          0x9e28e5d2, 0xc398c5d7, 0x1b635b55, 0xfd5f6264, 
          0xd8caedfb, 0xf791a9ec, 0x243cd93d, 0xa4ac8c5f, 
          0x8635bf62, 0x8607d9da, 0xc3328295, 0x5ccc6ab6, 
          0xb8b38f6d, 0x42eafc44, 0x426054f7, 0x51a1f31f, 
          0xece5363a, 0x58c767aa, 0x8b2d7203, 0xfe541e29, 
          0x9e84cab4, 0xe56841d3, 0x6d95e888, 0x458857e5, 
          0x6e85d0f4, 0x6085e614, 0x4cdaa48a, 0xb6bb861e, 
          0x64743a74, 0x451b3b40, 0xd501071b, 0xd87cd5a7, 
          0x371ad103, 0x5a3f9b65, 0x6281be7f, 0x8a4df4dc, 
          0xdbef1158, 0x84002b1c, 0x3e0e028b, 0x402cdd0b, 
          0x7957d0ae, 0x6f8ac3e8, 0x9c969e97, 0x17b3a671, 
          0x7e7163eb, 0x80db4a48, 0xbd45fb13, 0x84ea5100, 
          0x97634890, 0x14c3971c, 0x538f82aa, 0x16fb1e9e, 
          0x041958d2, 0xf1db2d3b, 0x65327104, 0x35c37fde, 
          0x7eaedd31, 0x863c2614, 0x9e0a6449, 0xcc74a47e, 
          0xc075361d, 0x5b48cd46, 0xe942c069, 0xf084d1cf, 
          0x93788a8c, 0xdfd12626, 0x7738febb, 0xf798942d, 
          0x9949abee, 0xa51844ae, 0x344f0028, 0x77bac45e, 
          0x6008850c, 0x2bfd89d5, 0x617eaae1, 0xfca631bf, 
          0xc03d610a, 0x78da5463, 0x26ccea65, 0x1c9859ac, 
          0xe6f865c8, 0x20b296b3, 0xccfb99bd, 0x012a99de, 
          0x0a7fa60f, 0x7c7455e8, 0xe73d0a98, 0xf1f341e7, 
          0xa478c622, 0xedd01c65, 0xccf6fef3, 0xcada0a34, 
          0xff746771, 0xeba5752a, 0xb366b97e, 0xa08ddaaf, 
          0xd1586ce0, 0xb2872f3a, 0x0afb62f3, 0xe6e885d4, 
          0xccc07b47, 0x2fffffe4, 0x13258cdf, 0x09d70ec3, 
          0x1af45dc6, 0x510bd0e2, 0x618619d7, 0x903d02b9, 
          0xf52338a0, 0x8ce8b20c, 0xb37ed714, 0xaa33568e, 
          0xf2b8ce92, 0x0b69937b, 0x8773645a, 0xf42fca3d, 
          0x0dab927b, 0x8d2d584e, 0x2111dcf4, 0x31423149, 
          0xdb1dab6d, 0x9b5d3bec, 0x230b57c3, 0xf47e6111, 
          0xabc60634, 0x48816aa3, 0xb2480a01, 0x8d837245, 
          0x6155d169, 0xd99cc22e, 0x2d54fc54, 0xbd629097, 
          0xd7665c4b, 0xcda3513d, 0x3a531933, 0x71dbdf04, 
          0x1f14762f, 0x8d0c4c16, 0xfb7d60d1, 0xc080c47b, 
          0xc24e84ac, 0xf9f0e0d3, 0xca91ac5d, 0xf127c9fc, 
          0x4e2f5071, 0x4f66bf58, 0xb0314452, 0xc404975a, 
          0xb1a991a6, 0x4863f443, 0xbf9d59f5, 0x1baaad0c, 
          0xa8a505ae, 0xae996b59, 0x2eabb518, 0x0b4bc0ae, 
          0x4b0ad14a, 0xac3c8521, 0x1aaacb46, 0x4b8b8ae4, 
          0xa5cd04b0, 0x8fcbbd0c, 0x03c4eabf, 0x6bd2a516, 
          0xf9271714, 0x0cc32266, 0x942a89d1, 0xefc0283b, 
          0x81864615, 0x63a78600, 0xa709c2c4, 0xe448d2d7, 
          0x76ede66c, 0x2d6dceb3, 0x0cc1852f, 0xc0b8e903, 
          0x06874033, 0xcb960909, 0xe2d89cfd, 0xc7e315c0, 
          0xfe5a0429, 0x9a127cd1, 0x0db0988d, 0x639f3e46, 
          0x3196b4d4, 0xd6128c08, 0xfb43c8e9, 0xc58babd6, 
          0xfe63ff94, 0x5d477b6d, 0xfe666cac, 0x23311e41, 
          0xb87dde76, 0xd22c3657, 0x004c4fc4, 0x1bb4d038, 
          0xdc9fc6cb, 0x4202a043, 0x85b8144a, 0xdd079973, 
          0xc37a9fac, 0xaaa488c5, 0x2e358dd1, 0xd563ffc6, 
          0x7c45ce3f, 0xaed2431e, 0xb7a7f400, 0x5f77f5f8, 
          0x86b82366, 0x821c4087, 0x312fe9a3, 0x5754cf7f, 
          0x2272944f, 0x85c54263, 0xc44c66a6, 0xf049699e, 
          0x283bcb22, 0xb3ec91f2, 0x3264cf3f, 0xbf43d322, 
          0xcff71869, 0xc7206582, 0x35eda983, 0x6222a791, 
          0x573142f9, 0x6487dfd1, 0x354e4b86, 0x79cd802b, 
          0xc9e8310e, 0x67a03c80, 0xa3e311a0, 0xd6a24fc0, 
          0x831bc6e2, 0xa3829fa7, 0x8be8e00e, 0x4b556770, 
          0x0c82f27c, 0x4564c4e2, 0x98fa208a, 0x22a5036e, 
          0x793fcde1, 0x51eeb74b, 0x66434985, 0xa320dabf, 
          0xaa193d0f, 0x2ac31170, 0x1a57408d, 0x878bf00b, 
          0xba17dd3f, 0xac204fff, 0x0326ae76, 0x110d4d70, 
          0xc86603f0, 0x137b5d1c, 0x579350dc, 0x87c8d087, 
          0x24eb34d0, 0x431325ec, 0x01c45887, 0x56b00c2a, 
          0xfc032eb8, 0xcc8e14d8, 0x7f82b0bc, 0x7db289a4, 
          0x2276d1b7, 0x7a9db672, 0xb5c0f846, 0x303d06ed, 
          0x9585888b, 0xc5f2b921, 0x8261602b, 0xf1b3083a, 
          0x0b7dc2ab, 0x334d1d72, 0x071e479c, 0x17abeb3f, 
          0x10b0af61, 0xe1fd1048, 0x5adca5ec, 0x82ba4730, 
          0xd1ec1a66, 0xf3a05096, 0xc4de5fae, 0xb855521f, 
          0x0faa54fa, 0x8b46d401, 0x1c5b3233, 0xc25fd785, 
          0x068c8cbc, 0x6d9b64df, 0x65a9678b, 0x28c1db76, 
          0x50259b3f, 0xfb88938f, 0x31cd82f3, 0xcbc13621, 
          0x0bf2ac3d, 0x4f4e6f12, 0x4a55e7dd, 0x879f182d, 
          0x461c9fd4, 0x254a718f, 0x88633911, 0xb496a391, 
          0xc5864f8b, 0xce9a9346, 0x4fb390a5, 0x099b7d21, 
          0x1bca7c18, 0xf5f03b2b, 0x08fb0969, 0xa5573065, 
          0x460db25f, 0x8d886540, 0xe29e44ae, 0xfa2ce107, 
          0xf9007e8d, 0x9f12ec1f, 0xb6242d3b, 0x855f0f1d, 
          0x2c482c9e, 0xd25ae14c, 0xd3f2e245, 0x823060a6, 
          0xeaafdc7b, 0x02b8e129, 0x0a5c3edc, 0x04d96ef8, 
          0xabeebb5c, 0x07a4def3, 0x5271d3e3, 0x00048070, 
          0x1e3da3bd, 0x1361538c, 0x7a4090f3, 0x7d263510, 
          0xef74685e, 0x185a2b84, 0xb82989bf, 0x91cc0141, 
          0x19fe9175, 0xd65f2733, 0xf12834d6, 0x42c5e75e, 
          0x0d6ca5d0, 0xf9650d2b, 0xa19efbed, 0xaf45d6d1 
};
int out_1[258] = {
          0x1751d056, 0xe61d1929, 0x32cd6d12, 0x81300f07, 
          0x9b26adbf, 0x1345ee81, 0xb512f013, 0x623704c4, 
          0xccd6468b, 0x5240b3f7, 0xaae32043, 0xd3eb2af1, 
          0x9d4d0b53, 0x79c6efa4, 0xa4c6c20d, 0xf9eccefa, 
          0xe251e0cd, 0x225deb1d, 0x3d43402a, 0x7ad99715, 
          0xf8d56e6e, 0xab7b44bf, 0x296a44cc, 0x60c8d3d1, 
          0x64b33e39, 0xe24a5318, 0xaab0d51a, 0xdc7e2e2b, 
          0xa2c3db30, 0xb965514d, 0x434dd996, 0xef16beeb, 
          0xf949c8b7, 0x15a71410, 0x1de183ac, 0x20a793fd, 
          0xeedbb1dd, 0xc34abd09, 0x152bc873, 0xc829e3ea, 
          0x6110f23b, 0xd7552b49, 0xb930e43a, 0x94d994cc, 
          0x2d0ba3a0, 0x0520b2cf, 0x378b9888, 0xab97aa71, 
          0xba6da924, 0xd48ad323, 0xeaae5cd1, 0x1620cfb4, 
          0x2d0025d1, 0xec0d42c3, 0xb0fa7617, 0x03493611, 
          0xe7ff6679, 0x385edc02, 0xbaaefd34, 0x6ddf6cf6, 
          0xe111a983, 0x9aa7edc9, 0xc2855e4a, 0xfd3489d3, 
          0xbe3c293b, 0xd0ca7f29, 0x5b4c82ac, 0x4882f9f2, 
          0x2fc4d6bc, 0xe9699ab7, 0xfa161603, 0x414c22e6, 
          0xc18072d8, 0x1fe1ecc6, 0xe7f08500, 0xbec0d5dd, 
          0xa3725290, 0x85787301, 0x1c82947e, 0xaab16e17, 
          0x07333788, 0xfbedff7a, 0x245997f8, 0x58764f12, 
          0x3f6ea8f8, 0x42b92fee, 0x25452d55, 0x34bcaaf0, 
          0x93b0bfe2, 0xc3d7b990, 0xe46eca6e, 0xfc11537b, 
          0x1af04448, 0x4011934a, 0x02360eb8, 0x8820ff2f, 
          0xfcc29a26, 0xfd3a78d4, 0x540a80b0, 0x55b0445a, 
          0x11ed4d9e, 0x02e9b99d, 0x4bb55482, 0x9335429f, 
          0x52a8ee0f, 0xaf3c44f4, 0x95bbacf7, 0x1b50f9f7, 
          0x38e6863a, 0x3efb333f, 0xa421f84b, 0x70aca846, 
          0x80b88b06, 0xb994a04a, 0x92cfc63c, 0x809694e4, 
          0x01fa6fdb, 0x5b1ba225, 0x83126994, 0x60311581, 
          0x06fb06d8, 0xd9182925, 0xa55c834e, 0xa6953a1c, 
          0x03d56453, 0x934f05c3, 0x5817b932, 0x6e33254f, 
          0x0b67b06e, 0x892ae33f, 0x932fbb7f, 0x177855aa, 
          0x330aa795, 0x58f550d2, 0xfb0036f5, 0x7e59b924, 
          0x8f7c88f6, 0x753912d6, 0x8f95ed71, 0x0eebe94e, 
          0x55e9075e, 0x634cd03f, 0x734f2658, 0xf5ee37c5, 
          0xb130cd77, 0x84502167, 0xbc63df17, 0x9f36fd7b, 
          0x58fa5039, 0x7b53c7cc, 0x53e749e6, 0x6a18877d, 
          0xfa93e21a, 0x4910e918, 0xa7817885, 0x98fd1314, 
          0x4563b653, 0x5c769e82, 0x6e12acc0, 0x29927af8, 
          0x004bd114, 0x0834ef7a, 0xf888e696, 0x746aafbf, 
          0x82d5f937, 0x399ade21, 0x8a087ad5, 0x4c6dffcd, 
          0x6ea67f29, 0xa0e61792, 0xa8be7f8c, 0x2c4de8ec, 
          0xffeaaf2e, 0xea29ac94, 0x3c8e0093, 0x67b635ec, 
          0x1cbf6e45, 0x42fce3dd, 0xe6183c63, 0x6cee210f, 
          0x04f5aca2, 0xb0792faf, 0x388bf774, 0xaa841bd9, 
          0x7d7faa87, 0x1233b4fc, 0x64b7ce00, 0xb208959f, 
          0x74cb100f, 0x1a621a0b, 0x8c127ec3, 0x768f492f, 
          0x7ad13007, 0xf622c79b, 0xb830fc1f, 0x5cdd54e0, 
          0x2f0f6e2a, 0x9c993a1b, 0x8b99d36b, 0xaf09cec5, 
          0x5b38ea03, 0x111547e3, 0xd5008aed, 0xdbdff5e0, 
          0x01b46b75, 0xfc6cbb9d, 0x0f908317, 0xc65fc6d3, 
          0x06f939e6, 0xe54203a1, 0x4bbca872, 0xe057e14b, 
          0xbe7624cf, 0x97f397ce, 0x56dc3020, 0x06261f8e, 
          0x7c0d195c, 0xf16fc6f1, 0xc9d4922e, 0x09e3ee8c, 
          0xafc1b445, 0x55fdbb0b, 0x3b3dd893, 0x7cfb12c5, 
          0x53b3f0ac, 0x15bb0563, 0x93cf68c3, 0xeff60c03, 
          0x08dddf85, 0x3eedefb0, 0x9dc8c674, 0xdcac3638, 
          0x75e9cda3, 0x70dbaeba, 0x392c5bb3, 0xc8ff4fb2, 
          0x49431aac, 0xed888d63, 0x257598ec, 0x8794e491, 
          0x3507e537, 0x01087572, 0xaff8f0db, 0x089fd803, 
          0xd33986b0, 0x71b3bd0e, 0x5cfa4cf6, 0x8ca90a66, 
          0x93bb28cd, 0xee67a1df, 0xc84c5a17, 0x0093e68a, 
          0xe91c4935, 0x01133184, 0x1dac076c, 0xd8966a20, 
          0x90a65215, 0x4fe08522 
};
int ref_2[14] = {
          0x42fce3dd, 0xe6183c63, 0x6cee210f, 0x04f5aca2, 
          0xb0792faf, 0x388bf774, 0xaa841bd9, 0x7d7faa87, 
          0x1233b4fc, 0x64b7ce00, 0xb208959f, 0x74cb100f, 
          0x1a621a0b, 0x8c127ec3 
};
int ref_3[2] = {
          0xb208959f, 0x74cb100f 
};
int out_4[1] = {
          0x2db355dc 
};
int out_5[3] = {
          0xf0d27745, 0x3fd02626, 0x935ffeeb 
};
int out_6[2] = {
          0xbbf8bc86, 0x296159e9 
};
int out_7[2] = {
          0xfc1432d8, 0x1adf8bd6 
};
int out_8[15] = {
          0x543051c6, 0xd18451b9, 0x8e5b3f53, 0x9a3bd470, 
          0xae7fcfe1, 0x8c8d677b, 0x7c74d55f, 0x0140cbeb, 
          0x11cc87a9, 0xabf806b3, 0x3d867c90, 0x3a998dcb, 
          0x220bf79a, 0x22e806e6, 0x7a5f1832 
};
int ref_9[1] = {
          0x1cbf6e45 
};
int ref_10[1] = {
          0x67b635ec 
};
int out_11[1] = {
          0x98eb5b9f 
};
int ref_12[1] = {
          0x2f0f6e2a 
};
int out_13[4] = {
          0x229bb295, 0xd3bc143d, 0x1a301483, 0x877e0059 
};
int out_14[21] = {
          0x21ac8aee, 0x22828226, 0x5b6aa127, 0x1d0ee353, 
          0xe973e4a3, 0x37857829, 0x2607bf42, 0x9e89846d, 
          0x739730ef, 0x6e8bd217, 0x9889824f, 0x3cf54c07, 
          0x4eda357b, 0xa0af9b79, 0x2dc01393, 0x5c5817b2, 
          0xeeb70499, 0x27928740, 0x2e7301b9, 0xad0a01c5, 
          0xb9a0a166 
};
int out_15[4] = {
          0x55c8016b, 0x5e5f4392, 0x528d9c66, 0x05076d4a 
};
int out_16[3] = {
          0xd531f1ac, 0xdaf6995d, 0xeea79b7b 
};
int out_17[9] = {
          0x6b25165b, 0x6f19ebf9, 0x5a1181db, 0xc688b9f1, 
          0x8da6dbb0, 0x68339c63, 0x1de0623a, 0x77a47736, 
          0x2e3e47a8 
};
int ref_18[4] = {
          0xea29ac94, 0x3c8e0093, 0x67b635ec, 0x1cbf6e45 
};
int out_19[1] = {
          0xa965c2c9 
};
int out_20[1] = {
          0x6b6281c7 
};
int ref_21[59] = {
          0xf5f03b2b, 0x08fb0969, 0xa5573065, 0x460db25f, 
          0x8d886540, 0xe29e44ae, 0xfa2ce107, 0xf9007e8d, 
          0x9f12ec1f, 0xb6242d3b, 0x855f0f1d, 0x2c482c9e, 
          0xd25ae14c, 0xd3f2e245, 0x823060a6, 0xeaafdc7b, 
          0x02b8e129, 0x0a5c3edc, 0x04d96ef8, 0xabeebb5c, 
          0x07a4def3, 0x5271d3e3, 0x00048070, 0x1e3da3bd, 
          0x1361538c, 0x7a4090f3, 0x7d263510, 0xef74685e, 
          0x185a2b84, 0xb82989bf, 0x91cc0141, 0x19fe9175, 
          0xd65f2733, 0xf12834d6, 0x42c5e75e, 0x0d6ca5d0, 
          0xf9650d2b, 0xa19efbed, 0xaf45d6d1, 0x1751d056, 
          0xe61d1929, 0x32cd6d12, 0x81300f07, 0x9b26adbf, 
          0x1345ee81, 0xb512f013, 0x623704c4, 0xccd6468b, 
          0x5240b3f7, 0xaae32043, 0xd3eb2af1, 0x9d4d0b53, 
          0x79c6efa4, 0xa4c6c20d, 0xf9eccefa, 0xe251e0cd, 
          0x225deb1d, 0x3d43402a, 0x7ad99715 
};
int ref_22[1] = {
          0x07333788 
};
int ref_23[40] = {
          0xf8d56e6e, 0xab7b44bf, 0x296a44cc, 0x60c8d3d1, 
          0x64b33e39, 0xe24a5318, 0xaab0d51a, 0xdc7e2e2b, 
          0xa2c3db30, 0xb965514d, 0x434dd996, 0xef16beeb, 
          0xf949c8b7, 0x15a71410, 0x1de183ac, 0x20a793fd, 
          0xeedbb1dd, 0xc34abd09, 0x152bc873, 0xc829e3ea, 
          0x6110f23b, 0xd7552b49, 0xb930e43a, 0x94d994cc, 
          0x2d0ba3a0, 0x0520b2cf, 0x378b9888, 0xab97aa71, 
          0xba6da924, 0xd48ad323, 0xeaae5cd1, 0x1620cfb4, 
          0x2d0025d1, 0xec0d42c3, 0xb0fa7617, 0x03493611, 
          0xe7ff6679, 0x385edc02, 0xbaaefd34, 0x6ddf6cf6 
};
int ref_24[33] = {
          0x296a44cc, 0x60c8d3d1, 0x64b33e39, 0xe24a5318, 
          0xaab0d51a, 0xdc7e2e2b, 0xa2c3db30, 0xb965514d, 
          0x434dd996, 0xef16beeb, 0xf949c8b7, 0x15a71410, 
          0x1de183ac, 0x20a793fd, 0xeedbb1dd, 0xc34abd09, 
          0x152bc873, 0xc829e3ea, 0x6110f23b, 0xd7552b49, 
          0xb930e43a, 0x94d994cc, 0x2d0ba3a0, 0x0520b2cf, 
          0x378b9888, 0xab97aa71, 0xba6da924, 0xd48ad323, 
          0xeaae5cd1, 0x1620cfb4, 0x2d0025d1, 0xec0d42c3, 
          0xb0fa7617 
};
int ref_25[9] = {
          0xec0d42c3, 0xb0fa7617, 0x03493611, 0xe7ff6679, 
          0x385edc02, 0xbaaefd34, 0x6ddf6cf6, 0xe111a983, 
          0x9aa7edc9 
};
int ref_26[41] = {
          0x2fc4d6bc, 0xe9699ab7, 0xfa161603, 0x414c22e6, 
          0xc18072d8, 0x1fe1ecc6, 0xe7f08500, 0xbec0d5dd, 
          0xa3725290, 0x85787301, 0x1c82947e, 0xaab16e17, 
          0x07333788, 0xfbedff7a, 0x245997f8, 0x58764f12, 
          0x3f6ea8f8, 0x42b92fee, 0x25452d55, 0x34bcaaf0, 
          0x93b0bfe2, 0xc3d7b990, 0xe46eca6e, 0xfc11537b, 
          0x1af04448, 0x4011934a, 0x02360eb8, 0x8820ff2f, 
          0xfcc29a26, 0xfd3a78d4, 0x540a80b0, 0x55b0445a, 
          0x11ed4d9e, 0x02e9b99d, 0x4bb55482, 0x9335429f, 
          0x52a8ee0f, 0xaf3c44f4, 0x95bbacf7, 0x1b50f9f7, 
          0x38e6863a 
};
int ref_27[1] = {
          0x3c8e0093 
};
int ref_28[1] = {
          0xbaaefd34 
};
int ref_29[3] = {
          0x4011934a, 0x02360eb8, 0x8820ff2f 
};
int ref_30[17] = {
          0x245997f8, 0x58764f12, 0x3f6ea8f8, 0x42b92fee, 
          0x25452d55, 0x34bcaaf0, 0x93b0bfe2, 0xc3d7b990, 
          0xe46eca6e, 0xfc11537b, 0x1af04448, 0x4011934a, 
          0x02360eb8, 0x8820ff2f, 0xfcc29a26, 0xfd3a78d4, 
          0x540a80b0 
};
int ref_31[3] = {
          0xe7ff6679, 0x385edc02, 0xbaaefd34 
};
int ref_32[1] = {
          0x6ddf6cf6 
};
int ref_33[1] = {
          0xa421f84b 
};
int ref_34[14] = {
          0xbaaefd34, 0x6ddf6cf6, 0xe111a983, 0x9aa7edc9, 
          0xc2855e4a, 0xfd3489d3, 0xbe3c293b, 0xd0ca7f29, 
          0x5b4c82ac, 0x4882f9f2, 0x2fc4d6bc, 0xe9699ab7, 
          0xfa161603, 0x414c22e6 
};
int out_35[1] = {
          0xc5d41f7c 
};
int ref_36[4] = {
          0x5b4c82ac, 0x4882f9f2, 0x2fc4d6bc, 0xe9699ab7 
};
int ref_37[173] = {
          0xc4de5fae, 0xb855521f, 0x0faa54fa, 0x8b46d401, 
          0x1c5b3233, 0xc25fd785, 0x068c8cbc, 0x6d9b64df, 
          0x65a9678b, 0x28c1db76, 0x50259b3f, 0xfb88938f, 
          0x31cd82f3, 0xcbc13621, 0x0bf2ac3d, 0x4f4e6f12, 
          0x4a55e7dd, 0x879f182d, 0x461c9fd4, 0x254a718f, 
          0x88633911, 0xb496a391, 0xc5864f8b, 0xce9a9346, 
          0x4fb390a5, 0x099b7d21, 0x1bca7c18, 0xf5f03b2b, 
          0x08fb0969, 0xa5573065, 0x460db25f, 0x8d886540, 
          0xe29e44ae, 0xfa2ce107, 0xf9007e8d, 0x9f12ec1f, 
          0xb6242d3b, 0x855f0f1d, 0x2c482c9e, 0xd25ae14c, 
          0xd3f2e245, 0x823060a6, 0xeaafdc7b, 0x02b8e129, 
          0x0a5c3edc, 0x04d96ef8, 0xabeebb5c, 0x07a4def3, 
          0x5271d3e3, 0x00048070, 0x1e3da3bd, 0x1361538c, 
          0x7a4090f3, 0x7d263510, 0xef74685e, 0x185a2b84, 
          0xb82989bf, 0x91cc0141, 0x19fe9175, 0xd65f2733, 
          0xf12834d6, 0x42c5e75e, 0x0d6ca5d0, 0xf9650d2b, 
          0xa19efbed, 0xaf45d6d1, 0x1751d056, 0xe61d1929, 
          0x32cd6d12, 0x81300f07, 0x9b26adbf, 0x1345ee81, 
          0xb512f013, 0x623704c4, 0xccd6468b, 0x5240b3f7, 
          0xaae32043, 0xd3eb2af1, 0x9d4d0b53, 0x79c6efa4, 
          0xa4c6c20d, 0xf9eccefa, 0xe251e0cd, 0x225deb1d, 
          0x3d43402a, 0x7ad99715, 0xf8d56e6e, 0xab7b44bf, 
          0x296a44cc, 0x60c8d3d1, 0x64b33e39, 0xe24a5318, 
          0xaab0d51a, 0xdc7e2e2b, 0xa2c3db30, 0xb965514d, 
          0x434dd996, 0xef16beeb, 0xf949c8b7, 0x15a71410, 
          0x1de183ac, 0x20a793fd, 0xeedbb1dd, 0xc34abd09, 
          0x152bc873, 0xc829e3ea, 0x6110f23b, 0xd7552b49, 
          0xb930e43a, 0x94d994cc, 0x2d0ba3a0, 0x0520b2cf, 
          0x378b9888, 0xab97aa71, 0xba6da924, 0xd48ad323, 
          0xeaae5cd1, 0x1620cfb4, 0x2d0025d1, 0xec0d42c3, 
          0xb0fa7617, 0x03493611, 0xe7ff6679, 0x385edc02, 
          0xbaaefd34, 0x6ddf6cf6, 0xe111a983, 0x9aa7edc9, 
          0xc2855e4a, 0xfd3489d3, 0xbe3c293b, 0xd0ca7f29, 
          0x5b4c82ac, 0x4882f9f2, 0x2fc4d6bc, 0xe9699ab7, 
          0xfa161603, 0x414c22e6, 0xc18072d8, 0x1fe1ecc6, 
          0xe7f08500, 0xbec0d5dd, 0xa3725290, 0x85787301, 
          0x1c82947e, 0xaab16e17, 0x07333788, 0xfbedff7a, 
          0x245997f8, 0x58764f12, 0x3f6ea8f8, 0x42b92fee, 
          0x25452d55, 0x34bcaaf0, 0x93b0bfe2, 0xc3d7b990, 
          0xe46eca6e, 0xfc11537b, 0x1af04448, 0x4011934a, 
          0x02360eb8, 0x8820ff2f, 0xfcc29a26, 0xfd3a78d4, 
          0x540a80b0, 0x55b0445a, 0x11ed4d9e, 0x02e9b99d, 
          0x4bb55482, 0x9335429f, 0x52a8ee0f, 0xaf3c44f4, 
          0x95bbacf7 
};
int ref_38[14] = {
          0x1de183ac, 0x20a793fd, 0xeedbb1dd, 0xc34abd09, 
          0x152bc873, 0xc829e3ea, 0x6110f23b, 0xd7552b49, 
          0xb930e43a, 0x94d994cc, 0x2d0ba3a0, 0x0520b2cf, 
          0x378b9888, 0xab97aa71 
};
int ref_39[4] = {
          0x55b0445a, 0x11ed4d9e, 0x02e9b99d, 0x4bb55482 
};
int ref_40[141] = {
          0x6d9b64df, 0x65a9678b, 0x28c1db76, 0x50259b3f, 
          0xfb88938f, 0x31cd82f3, 0xcbc13621, 0x0bf2ac3d, 
          0x4f4e6f12, 0x4a55e7dd, 0x879f182d, 0x461c9fd4, 
          0x254a718f, 0x88633911, 0xb496a391, 0xc5864f8b, 
          0xce9a9346, 0x4fb390a5, 0x099b7d21, 0x1bca7c18, 
          0xf5f03b2b, 0x08fb0969, 0xa5573065, 0x460db25f, 
          0x8d886540, 0xe29e44ae, 0xfa2ce107, 0xf9007e8d, 
          0x9f12ec1f, 0xb6242d3b, 0x855f0f1d, 0x2c482c9e, 
          0xd25ae14c, 0xd3f2e245, 0x823060a6, 0xeaafdc7b, 
          0x02b8e129, 0x0a5c3edc, 0x04d96ef8, 0xabeebb5c, 
          0x07a4def3, 0x5271d3e3, 0x00048070, 0x1e3da3bd, 
          0x1361538c, 0x7a4090f3, 0x7d263510, 0xef74685e, 
          0x185a2b84, 0xb82989bf, 0x91cc0141, 0x19fe9175, 
          0xd65f2733, 0xf12834d6, 0x42c5e75e, 0x0d6ca5d0, 
          0xf9650d2b, 0xa19efbed, 0xaf45d6d1, 0x1751d056, 
          0xe61d1929, 0x32cd6d12, 0x81300f07, 0x9b26adbf, 
          0x1345ee81, 0xb512f013, 0x623704c4, 0xccd6468b, 
          0x5240b3f7, 0xaae32043, 0xd3eb2af1, 0x9d4d0b53, 
          0x79c6efa4, 0xa4c6c20d, 0xf9eccefa, 0xe251e0cd, 
          0x225deb1d, 0x3d43402a, 0x7ad99715, 0xf8d56e6e, 
          0xab7b44bf, 0x296a44cc, 0x60c8d3d1, 0x64b33e39, 
          0xe24a5318, 0xaab0d51a, 0xdc7e2e2b, 0xa2c3db30, 
          0xb965514d, 0x434dd996, 0xef16beeb, 0xf949c8b7, 
          0x15a71410, 0x1de183ac, 0x20a793fd, 0xeedbb1dd, 
          0xc34abd09, 0x152bc873, 0xc829e3ea, 0x6110f23b, 
          0xd7552b49, 0xb930e43a, 0x94d994cc, 0x2d0ba3a0, 
          0x0520b2cf, 0x378b9888, 0xab97aa71, 0xba6da924, 
          0xd48ad323, 0xeaae5cd1, 0x1620cfb4, 0x2d0025d1, 
          0xec0d42c3, 0xb0fa7617, 0x03493611, 0xe7ff6679, 
          0x385edc02, 0xbaaefd34, 0x6ddf6cf6, 0xe111a983, 
          0x9aa7edc9, 0xc2855e4a, 0xfd3489d3, 0xbe3c293b, 
          0xd0ca7f29, 0x5b4c82ac, 0x4882f9f2, 0x2fc4d6bc, 
          0xe9699ab7, 0xfa161603, 0x414c22e6, 0xc18072d8, 
          0x1fe1ecc6, 0xe7f08500, 0xbec0d5dd, 0xa3725290, 
          0x85787301, 0x1c82947e, 0xaab16e17, 0x07333788, 
          0xfbedff7a 
};
int out_41[2] = {
          0x69d3a221, 0x392d9b37 
};
int out_42[1] = {
          0xddc34f7f 
};
int out_43[2] = {
          0x4b1bda67, 0x8278c3ee 
};
int out_44[3] = {
          0x2fcae44c, 0x00f25ad3, 0xf2a02d62 
};
int out_45[1] = {
          0xad27568d 
};
int out_46[6] = {
          0x69cd357f, 0xfb290473, 0xc9db7d00, 0x0bf38613, 
          0xaa22f5d7, 0x1c3bd059 
};
int out_47[1] = {
          0x6865fb4b 
};
int out_48[4] = {
          0x814d93cf, 0x85e02153, 0x3ef582a6, 0x09ce74c0 
};
int out_49[3] = {
          0xa5279970, 0x84f9ee04, 0x42026eb1 
};
int out_50[1] = {
          0x2a371def 
};
void func2(int *b)
{
    check(&b[760], ref_3, 2, "check_12 b", 760);
    copy(&b[742], out_4, 1);
    copy(&b[750], out_5, 3);
}
void func3(int *b)
{
    check(&b[760], ref_3, 2, "check_13 b", 760);
}
void func4(int *b)
{
    copy(&b[762], out_6, 2);
}
void func5(int *b)
{
    copy(&b[761], out_7, 2);
    copy(&b[728], out_8, 15);
}
void func1(int *b)
{
    check(&b[750], ref_2, 14, "check_7 b", 750);
    #pragma oss taskwait
    
    #pragma oss taskwait

    #pragma oss task in(b[760;2]) weakout(b[762;2]) out(b[742;1]) out(b[750;3]) node(0) label("func2")
    func2(b);

    #pragma oss task in(b[760;2]) weakinout(b[753;5]) node(0) label("func3")
    func3(b);

    #pragma oss task weakinout(b[759;1]) weakout(b[760;2]) out(b[762;2]) node(1) label("func4")
    func4(b);

    #pragma oss task out(b[761;2]) out(b[728;15]) weakinout(b[753;7]) node(0) label("func5")
    func5(b);
    
}
void func7(int *b)
{
    check(&b[749], ref_9, 1, "check_16 b", 749);
}
void func8(int *b)
{
}
void func9(int *a,int *b)
{
    check(&b[748], ref_10, 1, "check_19 b", 748);
}
void func10(int *a,int *b)
{
    check(&b[749], ref_9, 1, "check_20 b", 749);
}
void func6(int *a,int *b)
{
    #pragma oss taskwait

    #pragma oss task in(b[749;1]) node(0) label("func7")
    func7(b);

    #pragma oss task weakin(b[749;1]) node(0) label("func8")
    func8(b);

    #pragma oss taskwait

    #pragma oss task in(b[748;1]) weakout(a[884;19]) node(0) label("func9")
    func9(a,b);

    #pragma oss task in(b[749;1]) weakout(a[884;27]) node(1) label("func10")
    func10(a,b);

}
void func12(int *a,int *b)
{
    check(&b[769], ref_12, 1, "check_28 b", 769);
    copy(&a[884], out_13, 4);
}
void func13(int *b)
{
}
void func14(int *b)
{
}
void func15(int *a,int *b)
{
    copy(&a[893], out_14, 21);
}
void func11(int *a,int *b)
{
    #pragma oss task in(b[769;1]) out(a[884;4]) node(1) label("func12")
    func12(a,b);

    #pragma oss task weakin(b[769;1]) node(1) label("func13")
    func13(b);

    #pragma oss task weakin(b[769;1]) node(0) label("func14")
    func14(b);

    #pragma oss task weakin(b[769;1]) out(a[893;21]) node(0) label("func15")
    func15(a,b);

}
void func17(int *a)
{
}
void func18(int *a)
{
    copy(&a[864], out_15, 4);
}
void func19(int *b)
{
    copy(&b[884], out_16, 3);
}
void func20(int *b)
{
    copy(&b[880], out_17, 9);
}
void func16(int *a,int *b)
{
    #pragma oss task weakout(a[868;1]) weakout(a[846;6]) node(0) label("func17")
    func17(a);

    #pragma oss taskwait

    #pragma oss task out(a[864;4]) node(0) label("func18")
    func18(a);

    #pragma oss task out(b[884;3]) weakout(b[889;2]) node(0) label("func19")
    func19(b);

    #pragma oss task out(b[880;9]) node(0) label("func20")
    func20(b);

}
void func0(int *a,int *b)
{
    #pragma oss task inout(b[750;14]) weakout(b[721;24]) node(1) label("func1")
    func1(b);

    #pragma oss task weakout(a[882;29]) weakin(b[748;2]) node(1) label("func6")
    func6(a,b);

    #pragma oss taskwait

    #pragma oss taskwait

    copy(&a[911], out_11, 1);
    #pragma oss task weakout(a[792;132]) weakin(b[769;1]) node(1) label("func11")
    func11(a,b);

    #pragma oss task weakout(b[876;15]) weakout(a[820;49]) node(1) label("func16")
    func16(a,b);

}
void func23(int *b)
{
    copy(&b[749], out_20, 1);
}
void func24(int *b)
{
}
void func25(int *b)
{
    check(&b[534], ref_21, 59, "check_39 b", 534);
    check(&b[653], ref_22, 1, "check_40 b", 653);
    check(&b[593], ref_23, 40, "check_41 b", 593);
}
void func26(int *b)
{
    check(&b[595], ref_24, 33, "check_42 b", 595);
}
void func22(int *b)
{
    #pragma oss taskwait

    #pragma oss taskwait

    #pragma oss task weakin(b[542;6]) weakin(b[653;1]) out(b[749;1]) weakin(b[610;11]) node(0) label("func23")
    func23(b);

    #pragma oss task weakin(b[653;1]) weakin(b[569;74]) node(0) label("func24")
    func24(b);

    #pragma oss task in(b[534;59]) in(b[653;1]) in(b[593;40]) node(1) label("func25")
    func25(b);

    #pragma oss task weakin(b[653;1]) in(b[595;33]) node(0) label("func26")
    func26(b);

}
void func28(int *b)
{
    check(&b[747], ref_27, 1, "check_55 b", 747);
    check(&b[631], ref_28, 1, "check_56 b", 631);
    check(&b[666], ref_29, 3, "check_57 b", 666);
}
void func29(int *b)
{
    check(&b[747], ref_27, 1, "check_58 b", 747);
}
void func30(int *b)
{
    check(&b[655], ref_30, 17, "check_59 b", 655);
    check(&b[747], ref_27, 1, "check_60 b", 747);
}
void func31(int *b)
{
    check(&b[629], ref_31, 3, "check_61 b", 629);
    check(&b[632], ref_32, 1, "check_62 b", 632);
}
void func27(int *b)
{
    check(&b[626], ref_25, 9, "check_47 b", 626);
    check(&b[641], ref_26, 41, "check_48 b", 641);
    #pragma oss taskwait

    #pragma oss taskwait

    #pragma oss task in(b[747;1]) in(b[631;1]) weakin(b[626;1]) in(b[666;3]) node(1) label("func28")
    func28(b);

    #pragma oss task weakin(b[679;1]) in(b[747;1]) weakin(b[647;3]) node(1) label("func29")
    func29(b);

    #pragma oss task in(b[655;17]) weakin(b[681;1]) in(b[747;1]) node(0) label("func30")
    func30(b);

    #pragma oss task weakin(b[668;2]) in(b[629;3]) in(b[632;1]) node(1) label("func31")
    func31(b);

}
void func33(int *b)
{
    check(&b[631], ref_34, 14, "check_64 b", 631);
    check(&b[746], out_19, 1, "check_65 b", 746);
    copy(&b[746], out_35, 1);
}
void func34(int *b)
{
    check(&b[683], ref_33, 1, "check_66 b", 683);
}
void func35(int *b)
{
    check(&b[683], ref_33, 1, "check_67 b", 683);
}
void func36(int *b)
{
    check(&b[639], ref_36, 4, "check_68 b", 639);
}
void func32(int *b)
{
    check(&b[683], ref_33, 1, "check_63 b", 683);
    #pragma oss task weakin(b[615;16]) in(b[631;14]) inout(b[746;1]) node(1) label("func33")
    func33(b);

    #pragma oss task weakout(b[747;1]) in(b[683;1]) weakout(b[748;1]) node(1) label("func34")
    func34(b);

    #pragma oss task in(b[683;1]) weakout(b[747;1]) node(0) label("func35")
    func35(b);

    #pragma oss task weakin(b[645;1]) weakin(b[630;1]) in(b[639;4]) weakin(b[683;1]) node(1) label("func36")
    func36(b);

}
void func38(int *b)
{
    check(&b[607], ref_38, 14, "check_72 b", 607);
}
void func39(int *b)
{
    check(&b[672], ref_39, 4, "check_75 b", 672);
}
void func40(int *b)
{
    check(&b[514], ref_40, 141, "check_76 b", 514);
    copy(&b[746], out_41, 2);
}
void func41(int *b)
{
}
void func37(int *b)
{
    check(&b[507], ref_37, 173, "check_69 b", 507);
    #pragma oss taskwait

    #pragma oss task in(b[607;14]) weakin(b[621;37]) node(1) label("func38")
    func38(b);

    #pragma oss taskwait

    #pragma oss task weakin(b[555;25]) in(b[672;4]) node(1) label("func39")
    func39(b);

    #pragma oss task in(b[514;141]) out(b[746;2]) node(1) label("func40")
    func40(b);

    #pragma oss task weakout(b[746;1]) node(1) label("func41")
    func41(b);

}
void func21(int *b)
{
    check(&b[746], ref_18, 4, "check_31 b", 746);
    copy(&b[746], out_19, 1);
    #pragma oss task weakin(b[511;142]) weakin(b[653;1]) weakout(b[748;2]) node(0) label("func22")
    func22(b);

    #pragma oss taskwait

    #pragma oss taskwait

    #pragma oss task weakin(b[746;2]) in(b[626;9]) in(b[641;41]) node(1) label("func27")
    func27(b);

    #pragma oss task out(b[747;2]) in(b[683;1]) weakin(b[610;39]) weakinout(b[746;1]) node(0) label("func32")
    func32(b);

    #pragma oss task out(b[746;2]) in(b[507;173]) node(0) label("func37")
    func37(b);

}
void func44(int *a,int *b)
{
    copy(&b[616], out_42, 1);
    copy(&a[475], out_43, 2);
}
void func45(int *a)
{
    copy(&a[474], out_44, 3);
}
void func46(int *b)
{
    copy(&b[616], out_45, 1);
}
void func47(int *a)
{
    check(&a[467], out_46, 6, "check_78 a", 467);
}
void func43(int *a,int *b)
{
    #pragma oss task out(b[616;1]) weakout(a[480;1]) out(a[475;2]) node(1) label("func44")
    func44(a,b);

    #pragma oss task out(a[474;3]) weakout(a[473;1]) node(1) label("func45")
    func45(a);

    #pragma oss task out(b[616;1]) node(1) label("func46")
    func46(b);

    copy(&a[467], out_46, 6);
    #pragma oss task in(a[467;6]) weakout(a[473;7]) node(1) label("func47")
    func47(a);

}
void func49(int *b)
{
    copy(&b[610], out_47, 1);
}
void func50(int *b)
{
    copy(&b[614], out_48, 4);
}
void func51(int *b)
{
    copy(&b[610], out_49, 3);
}
void func52(int *b)
{
    copy(&b[612], out_50, 1);
}
void func48(int *b)
{
    #pragma oss task out(b[610;1]) weakout(b[614;1]) node(0) label("func49")
    func49(b);

    #pragma oss taskwait

    #pragma oss task out(b[614;4]) weakout(b[612;1]) node(1) label("func50")
    func50(b);

    #pragma oss task out(b[610;3]) weakout(b[613;2]) node(0) label("func51")
    func51(b);

    #pragma oss task out(b[612;1]) weakout(b[617;1]) node(1) label("func52")
    func52(b);

}
void func54(int *a)
{
}
void func53(int *a,int *b)
{
    #pragma oss task weakout(a[517;43]) node(1) label("func54")
    func54(a);

}
void func42(int *a,int *b)
{
    #pragma oss task out(b[616;1]) out(a[466;15]) node(1) label("func43")
    func43(a,b);

    #pragma oss task weakout(b[610;8]) node(0) label("func48")
    func48(b);

    #pragma oss task weakout(a[656;46]) out(b[601;21]) out(a[492;108]) node(1) label("func53")
    func53(a,b);

}

bool fail12()
{
    int *a = nanos6_dmalloc(1000 * sizeof(int), nanos6_equpart_distribution, 0, NULL);
    int *b = nanos6_lmalloc(1000 * sizeof(int));

    copy(&b[157], out_0, 416);
    #pragma oss taskwait

    copy(&b[573], out_1, 258);
    #pragma oss taskwait

    #pragma oss task weakinout(b[691;79]) out(b[847;44]) out(a[755;185]) node(0) label("func0")
    func0(a,b);

    #pragma oss task inout(b[746;4]) weakin(b[345;339]) node(0) label("func21")
    func21(b);

    #pragma oss task weakout(b[601;25]) weakout(a[409;302]) node(1) label("func42")
    func42(a,b);
    
    #pragma oss taskwait
    printf("Test passed [./nasty.py --nodes 2 --tasks 55 --nesting 3 --seed 8 --no-hash]\n");
    return true;
}
    
int main(int argc, char *argv[])
{
	test_t test = FUNC_DEF(fail12);

	return run_test(&test);
}
