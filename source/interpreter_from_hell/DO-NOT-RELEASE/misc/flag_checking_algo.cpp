#include <stdio.h>
#include <random>

using namespace std;

int main()
{
    unsigned char flag[] = "wgmy{10e3b9a7cb5a88391afb2a1c56c5ac98}";

    //
    // part 1 - checksum
    //

    unsigned int a=0;
    for (int i=0; i<sizeof(flag); i++) {
        a = a ^ (flag[i] << (i % 32));
    }

    if (a != 0xb5973a46)
        return 0;

    //
    // part 2
    //

    if (
        !(
            ('f'^0x11) == flag[0] && 
            ('l'^0x0b) == flag[1] && 
            ('a'^0x0c) == flag[2] && 
            ('g'^0x1e) == flag[3] && 
            (':'^0x41) == flag[4] &&
            ('('^0x55) == flag[37]
        )
    ) {
        return 0;
    }


    //
    // part 3
    //

    unsigned char flag_part2[16];
    for (int j=5; j < 21; j++) flag_part2[j-5] = flag[j];

    int rand_seed = (flag[0]<<24) ^ (flag[1]<<16) ^ (flag[2]<<8) ^ flag[3];

    // mt19937 generated sequences are consistent across platforms/compilers
    // refer: http://anadoxin.org/blog/c-shooting-yourself-in-the-foot-4.html
    mt19937 mt_rand(rand_seed);

    for (int i = 0; i < 125; i++) {
        for (int j=0; j < 16; j++) flag_part2[j] = (flag_part2[j] ^ mt_rand()) & 0xff;
        for (int j=1; j < 16; j++) flag_part2[j] ^= flag_part2[j-1];
    }

    int part2[] = {0xa0, 0xb, 0x77, 0xf1, 0xb2, 0x4b, 0x6e, 0x63,
                    0xef, 0xfd, 0xaa, 0x8e, 0xd9, 0xce, 0x50, 0x9c};
    
    for (int j=0; j < 16; j++)
        if (flag_part2[j] != part2[j])
            return 0;

    //
    // part 4
    //

    if (
        !(
            flag[11] == flag[26] &&
            flag[35] ^ flag[36] ^ flag[31] ^ flag[34] == 1 &&
            flag[26] ^ flag[36] ^ flag[35] ^ flag[21] == 81 &&
            flag[31] ^ flag[22] ^ flag[23] ^ flag[27] == 85 &&
            flag[30] ^ flag[25] ^ flag[22] ^ flag[34] == 6 &&
            flag[21] ^ flag[29] ^ flag[24] ^ flag[26] == 7 &&
            flag[25] ^ flag[23] ^ flag[36] == 108 &&
            flag[36] ^ flag[35] ^ flag[25] == 51 &&
            flag[29] ^ flag[32] ^ flag[33] ^ flag[21] == 80 &&
            flag[25] ^ flag[26] ^ flag[30] ^ flag[34] == 6 &&
            flag[21] ^ flag[24] ^ flag[34] == 48 &&
            flag[29] ^ flag[35] ^ flag[30] ^ flag[27] == 11 &&
            flag[34] ^ flag[32] ^ flag[23] ^ flag[30] == 6 &&
            flag[33] ^ flag[23] ^ flag[26] ^ flag[35] == 95 &&
            flag[32] ^ flag[33] ^ flag[30] == 98 &&
            flag[27] ^ flag[28] ^ flag[23] ^ flag[30] == 2
        )
    ) {
        return 0;
    }

    printf("success\n\n");

}