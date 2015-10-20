

#include "des.h"
#include <stdio.h>
#include <assert.h>

int main()
{
    uint64_t k = 0x0123456789abcdef;
    int i;
    for(i=1; i <= 16; i++){
        k = KS(i, k);
        printf("K%.2d: ", i);
        print_bits(k, 48, 6);
    }
    return 0;
}
