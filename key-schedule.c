#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

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

uint64_t IP(uint64_t block, uint8_t table[], int num){
    uint64_t out, tmp=1, result=0;
    int i, shift;
    for(i=0; i<num; i++){
        shift = (table[i]-1-i);
        if(shift > 0)
            out = (block << shift) & (tmp << (63-i));
        else
            out = (block >> (-1 * shift)) & (tmp << (63-i));
        result |= out;
//        printf("B: %.16lx\n", b);
 //       printf("IP: %.16lx\n", result);
//        printf("move %d to %d\n", IP_TABLE[i], (i+1));
  //      print_bits(block, 64);
    //    print_bits(b, 64);
//        print_bits(result, i+1);
       // getchar();
    }
    return result;
}

void print_bits(uint64_t b, int len, int p){
    int i;
    uint64_t t=1;
    for(i=0; i<len; i++){
        if(b & (t<<63-i))
            printf("%d",1);
        else
            printf("%d",0);

        if((i+1)%p == 0)
            printf(" ");
    }
    printf("\n");
}

#define CK(k) ((k) & 0xffffffff00000000)
#define DK(k)  (((k)<<28) & 0xffffffff00000000)

uint64_t left_one(uint64_t b){
    if(b & 0x8000000000000000)
        return (b << 1) | 0x0000001000000000;
    else
        return b << 1;

}

struct key_pair {
    uint64_t c_key;
    uint64_t d_key;
};

#define Merge_key_pair(pair)  ((pair).c_key | ((pair).d_key >> 28))

struct key_pair 
KS(int n, uint64_t key){
    static uint64_t c_keys[17] = {0, 0,};
    static uint64_t d_keys[17] = {0, 0,};
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

    result = KS(n-1, key);
    int i;
    for(i=0; i<shift_table[n-1]; i++){
        result.c_key = left_one(result.c_key);
        result.d_key = left_one(result.d_key);
    }
    c_keys[n] = result.c_key;
    d_keys[n] = result.d_key;
    return result;

}

int main()
{
    uint64_t k = 0x0123456789abcdef, k1, c, d;
    k1 = IP(k,PC1, sizeof(PC1));
    //print_bits(k, 64, 8);
    //print_bits(k1, 56, 7);
    
    //c = CK(k1);
    //d = DK(k1);
    //print_bits(c, 28, 7);
    //print_bits(d, 28, 7);
    int i;
    struct key_pair r;
    for(i=1; i <= 16; i++){
        r = KS(i, k1);
        k = Merge_key_pair(r);
        k = IP(k,PC2, sizeof(PC2));
        printf("K%.2d: ", i);
        print_bits(k, 48, 6);
        //printf("C%.2d: ", i);
       // print_bits(r.c_key, 28, 7);
      //  printf("D%.2d: ", i);
     //   print_bits(r.d_key, 28, 7);
    }
    return 0;
}
