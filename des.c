#include "des.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>


#define L(b) ((b) & 0xffffffff00000000)
#define R(b) (((b) << 32) & 0xffffffff00000000)
#define MEGER(l,r) ((l) | ((r) >> 32)) 

int des(u64 key, u64 msg, int flag)
{
    u64 k;//, key = 0x1111111111111111;// 0x133457799bbcdff1;
    u64 r; //msg = 0x0123456789abcdef;
    u32 L0, R0, L1, R1, tmp;

    msg = init_permute(msg);
    L0 = L(msg);
    R0 = R(msg);

    int i, n;
    for(i=0; i<16; i++){
        n = flag ? i + 1 : 16 - i;
        k = KS(n, key);
        L1 = R0;
        R1 = (L0 ^ f(R0,k));
        R0 = R1;
        L0 = L1;
    }
    r = MEGER(R1, L1);
    r = final_permute(r);
    if(flag)
        printf("Encrypting\n");
    else
        printf("Dncrypting\n");

    printf("Given Key: 0x%.16lx\n", key);
    printf("Given Message: 0x%.16lx\n", msg);
    printf("Output: 0x%.16lx\n", r);
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


    /*
    print_bits(init_permute(msg), 64, 8);
    int i;
    for(i=1; i <= 16; i++){
        k = KS(i, key);
        printf("K%.2d: ", i);
        print_bits(k, 48, 6);
    }
    */

