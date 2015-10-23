#include <stdint.h>

typedef uint64_t u64;
typedef uint64_t u56;
typedef uint64_t u48;
typedef uint64_t u32;
typedef uint64_t u28;

void print_bits(u64 b, int len, int p);
u64 permute(u64 block, uint8_t table[], int num);
u64 init_permute(u64 block);
u64 final_permute(u64 block);

/* key-schedul */
u48 KS(int n, u64 key);

/* f.c */

u32 f(u32 R, u48 key);


