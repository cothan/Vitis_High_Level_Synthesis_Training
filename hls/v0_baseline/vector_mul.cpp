#include "vector_mul.h"

// Baseline
u32 hls_vector_mul(const u32 a[N], const u32 b[N], const u32 c[N])
{
    const u32 mask = 0x1fffffff;
    u32 sum = 0;
    for (auto i = 0; i < N; i++)
    {
        sum += (c[i] + a[i] * b[i]) & mask;
    }
    sum &= mask;
    sum *= ALPHA;

    return sum;
}
