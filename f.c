#include "des.h"
#include <stdio.h>


//& 0x00000000ffffffff


/*Expend */
static uint8_t E_table[48] = {
32, 1, 2, 3, 4, 5,
4, 5, 6, 7, 8, 9,
8, 9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32, 1,
};

u48 E(u32 R){
    return permute(R, E_table, sizeof(E_table));
}




/* S-box  */
typedef uint8_t sbox[4][16];

static sbox S_table[8] = { 
{    
14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
},
{
15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
},
{
10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
},
{
7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
},
{
2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
},
{
12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
},
{
4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
},
{
13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11,
}
};


#define GET_6bits(b, i)  (((b) >> 64-i*6) & 0x000000000000003f)

int get_row(uint8_t b){
    b &= 0x21;
    switch(b){
        case 0b000000:
            return 0;
        case 0b000001:
            return 1;
        case 0b100000:
            return 2;
        case 0b100001:
            return 3;
    }
}

int get_col(uint8_t b){
    b = ((b >> 1) & 0x0f);
    return b;
}

u32 S_box(u48 b){
    int row, col;
    uint8_t bit6;
    u32 result=0, tmp;

    int i;
    for(i=1; i<=8; i++){
        bit6 = GET_6bits(b,i);
        row = get_row(bit6);
        col = get_col(bit6);
        tmp = (u32)S_table[i-1][row][col];
//      printf("%dbits:%x row=%d  col=%d  val=%d\n",i, bit6, row, col, tmp);
        result |= (tmp << (64-i*4));

    }
    return result;
}


/* P permutation */

static uint8_t P_table[32] = {
16, 7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2, 8, 24, 14,
32, 27, 3, 9,
19, 13, 30, 6,
22, 11, 4, 25,
};

u32 P(u32 b){
    return permute(b, P_table, sizeof(P_table));
}


/* f function */

u32 f(u32 R, u48 key){
    R = E(R);
    u48 tmp = (R ^ key);
    tmp = S_box(tmp);
    return P(tmp);
}

/*
//Encrypt
int main()
{
    u64 k,k1, key = 0x1111111111111111;// 0x133457799bbcdff1;
    u64 r, msg = 0x0123456789abcdef;
    u32 L0, R0, L1, R1, tmp;

    msg = init_permute(msg);
    L0 = L(msg);
    R0 = R(msg);

    int i;
    for(i=0; i<16; i++){
        k = KS(i+1, key);
        L1 = R0;
        R1 = (L0 ^ f(R0,k));
        R0 = R1;
        L0 = L1;
    }
    //print_bits(L1, 32, 4);
    //print_bits(R1, 32, 4);
    
    r = MEGER(R1, L1);
    //print_bits(r, 64, 8);
    r = final_permute(r);
    //print_bits(r, 64, 8);
    printf("%.16lx\n", r);
    return 0;
}
*/
