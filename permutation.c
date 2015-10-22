#include "des.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


static uint8_t IP_TABLE[64] = {
58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 10, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17,  9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7,
};

static uint8_t IP1_TABLE[64] = {
40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41, 9,  49, 17, 57, 25,
};

u64 permute(u64 block, uint8_t table[], int num){
    u64 out, tmp=1, result=0;
    int i, shift;
    for(i=0; i<num; i++){
        shift = (table[i]-1-i);
        if(shift > 0)
            out = (block << shift) & (tmp << (63-i));
        else
            out = (block >> (-1 * shift)) & (tmp << (63-i));
        result |= out;
    }
    return result;
}

u64 init_permute(u64 block){
    return permute(block, IP_TABLE, sizeof(IP_TABLE));

}

u64 final_permute(u64 block){
    return permute(block, IP1_TABLE, sizeof(IP1_TABLE));

}

void print_bits(u64 b, int len, int p){
    int i;
    u64 t=1;
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

/*
u64 IP11(u64 block, uint8_t table[]){
    u64 out, tmp=1, result=0;
    int i, shift;
    for(i=0; i<64; i++){
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


void test(){
    u64 t, b, tmp=1, result=0;
    int i = 29;
    u64 block = 0x0123456789abcdef;

    printf("move %d to %d\n", IP_TABLE[i], (i+1));
    print_bits(block, 64);


    int p = IP_TABLE[i]-1-i;
    p = -16;
    printf("<< %d\n",p);
    t = block << p;
    print_bits(t, 64);

    t = tmp << (63-i);
    print_bits(t, 64);

    b = (block << (IP_TABLE[i]-1-i)) & (tmp << (63-i));
    print_bits(b, 64);
    exit(1);
    assert(b);
}



void test_shift(){
    int i;
    u64 b,tmp = 1;
    for(i=0; i<64; i++){
        b = tmp << (63-i);
        printf("B: %.16lx\n", b);
        getchar();
    }

}

void test_bshift(){
    int i;
    u64 b,block=0xffffffffffffffff;
    for(i=0; i<64; i++){
        b = (block << (IP_TABLE[i]-1));
        printf("B: %.16lx\n", b);
        getchar();
    }

}

void test_lrshift()
{
    u64 b, c, block = 0x0123456789abcdef, tmp=1;
    int i, shift;
    for(i=0; i<63; i++){
        shift = IP_TABLE[i]-1-i;
        b = (block << shift) & (tmp << (63-i));
        c = (block >> (-1 * shift)) & (tmp << (63-i));

        print_bits(block, 64);

        printf("<< %d\n",shift);
        print_bits(b, 64);

        printf(">> -%d\n",shift);
        print_bits(c, 64);

        getchar();
        assert(b == c);
    }
}

void print_bits(u64 b, int len){
    int i;
    u64 t=1;
    for(i=0; i<len; i++){
        if(b & (t<<63-i))
            printf("%d",1);
        else
            printf("%d",0);

        if((i+1)%8 == 0)
            printf(" ");
    }
    printf("\n");
}
int main(int argc, char **argv)
{
//   test_shift();
//   test_bshift();
//   test_lrshift();
//    test();
    u64 b = 0x0123456789abcdef;
    u64 rb;
    rb = IP(b, IP_TABLE);
    printf("IP: %.16lx\n", b);
    printf("IP: %.16lx\n", rb);
    print_bits(b, 64);
    print_bits(rb, 64);
    return 0;
}*/
