/* key schedule
 *    1.Initial permutaiton (64 bits to 56)
 *    2.spliting to c, d (each 28 bits)
 *    3.shifts
 *    4.Final permutation (56 bits to 48)
 *
 */

#include "des.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

u64 left_one(u64 b);
u64 KS1(int n, u64 key);


static uint8_t PC1[56]= {
57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10, 2, 59, 51, 43, 35, 27,
19, 11, 3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14, 6, 61, 53, 45, 37, 29,
21, 13, 5, 28, 20, 12, 4,
};

static uint8_t PC2[48]= {
14, 17, 11, 24, 1, 5, 
3, 28, 15, 6, 21, 10,
23, 19, 12, 4, 26, 8,
16, 7, 27, 20, 13, 2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32,
};



#define CK(k) ((k) & 0xfffffff000000000)
#define DK(k)  (((k)<<28) & 0xfffffff000000000)

u64 left_one(u64 b){
    if(b & 0x8000000000000000)
        return (b << 1) | 0x0000001000000000;
    else
        return b << 1;

}

u48 KS(int n, u64 key){
    u64 k;
    k = permute(key, PC1, sizeof(PC1));
    k = KS1(n, k);
    k = permute(k, PC2, sizeof(PC2));
    return k;
}

u56 KS1(int n, u64 key){
    static u64 keys[17] = {0, 0,};
    static uint8_t shift_table[16] = {
                                1, 1, 2, 2, 2, 2, 2, 2,
                                1, 2, 2, 2, 2, 2, 2, 1
                              };
    u64 c, d, result;

    if(keys[n] != 0)
        return keys[n];

    if(n == 0){
        c = CK(key);
        d = DK(key);
        keys[0] = (c | (d >> 28));
        return keys[0];
    }

    result = KS1(n-1, key);
    c = CK(result);
    d = DK(result);
    int i;
    for(i=0; i<shift_table[n-1]; i++){
        c = left_one(c);
        d = left_one(d);
    }
    keys[n] = (c | (d >> 28));
    return keys[n];

}


struct key_pair {
    u64 c_key;
    u64 d_key;
};

#define Merge_key_pair(pair)  ((pair).c_key | ((pair).d_key >> 28))

struct key_pair 
KS_pair(int n, u64 key){
    static u64 c_keys[17] = {0, 0,};
    static u64 d_keys[17] = {0, 0,};
    static uint8_t shift_table[16] = {
                                1, 1, 2, 2, 2, 2, 2, 2,
                                1, 2, 2, 2, 2, 2, 2, 1
                                };
    struct key_pair result;
    if(c_keys[n] != 0){
        result.c_key = c_keys[n];
        result.d_key = d_keys[n];
        return result;
    }

    if(n == 0){
        c_keys[0] = CK(key);
        d_keys[0] = DK(key);
        result.c_key = c_keys[0];
        result.d_key = d_keys[0];
        return result;
    }

    result = KS_pair(n-1, key);
    int i;
    for(i=0; i<shift_table[n-1]; i++){
        result.c_key = left_one(result.c_key);
        result.d_key = left_one(result.d_key);
    }
    c_keys[n] = result.c_key;
    d_keys[n] = result.d_key;
    return result;

}
/*
int main()
{
    u64 k = 0x0123456789abcdef, k1, c, d, result;
    k1 = permute(k,PC1, sizeof(PC1));
    int i;

    struct key_pair r;
    for(i=0; i <= 16; i++){
        r = KS_pair(i, k1);
        d = Merge_key_pair(r);
        d = permute(d,PC2, sizeof(PC2));
        c = KS(i, k);
        assert(d == c);
        printf("K%.2d: ", i);
        print_bits(c, 48, 6);
    }
    return 0;
}
*/
