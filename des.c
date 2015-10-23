#include "des.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L(b) ((b) & 0xffffffff00000000)
#define R(b) (((b) << 32) & 0xffffffff00000000)
#define MEGER(l,r) ((l) | (((r) >> 32) & 0x00000000ffffffff)) 

int des(u64 key, u64 msg, int flag)
{
    u64 k, r, m;
    u32 L0, R0, L1, R1;

    m = init_permute(msg);
    L0 = L(m);
    R0 = R(m);

#ifdef DEBUG
    printf("Initial permutation:\n");
    print_bits(m, 64, 8);

    printf("L0: ");
    print_bits(L0, 32, 8);
    printf("R0: ");
    print_bits(R0, 32, 8);
#endif

    int i, n;
    for(i=0; i<16; i++){
        n = flag ? i + 1 : 16 - i;
        k = KS(n, key);
        L1 = R0;
        R1 = (L0 ^ f(R0,k));
        R0 = R1;
        L0 = L1;
#ifdef DEBUG
    printf("Key%.2d: ",n);
    print_bits(k, 48, 6);
    if(i == 0){
        printf("after 1st iteration:\n");
        printf("L1: ");
        print_bits(L0, 32, 8);
        printf("R1: ");
        print_bits(R0, 32, 8);
    }
    if(i == 15){
        printf("after 16st iteration:\n");
        printf("L16: ");
        print_bits(L0, 32, 8);
        printf("R16: ");
        print_bits(R0, 32, 8);
    }
#endif
    }
    r = MEGER(R1, L1);
    r = final_permute(r);

    if(flag)
        printf("Encrypting...\n");
    else
        printf("Decrypting...\n");

    printf("\tGiven Key: 0x%.16lx\n", key);
    printf("\tGiven Message: 0x%.16lx\n", msg);
    printf("\tOutput: 0x%.16lx\n", r);
    return 0;
}

int main(int argc, char **argv)
{
    int opt, flag=1;
    u64 key = 0x0123456789abcdef;
    u64 msg = 0x0123456789abcdef;

    if(argc != 4 && argc != 3){
        printf("usage: ./des [-d|-t] key msg\n");
        printf("    -e    encrypt\n");
        printf("    -d    decrypt\n");
        printf("default is encrypt\n");
        exit(1);
    }

    if( (opt = getopt(argc, argv, "ed")) != -1){
        if(opt == 'd')
            flag = 0;
        if(opt == '?')
            exit(2);
    }
    sscanf(argv[argc-2],"%lx", &key);
    sscanf(argv[argc-1],"%lx", &msg);
    des(key, msg, flag);

//    printf("%.16lx\n", key);
//    printf("%.16lx\n", msg);
    return 0;
}


