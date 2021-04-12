#include "vector_mul.h"

u32 hls_vector_mul(const u32 a[N], const u32 b[N], const u32 c[N])
{
    const u32 mask = 0x1fffffff;
    u32 sum = 0;
    for (auto i = 0; i < N; i++)
    {
#pragma HLS UNROLL
        sum += (c[i] + a[i] * b[i]) & mask;
    }
    sum &= mask;
    sum *= ALPHA;

    return sum;
}

// ============================
/*
We cannot unroll the the who equation with parameter N = 200, it's too big
Cons: Huge adder tree. 
*/

