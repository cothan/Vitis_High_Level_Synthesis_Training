#include "vector_mul.h"

#define BUFFER 40

u32 adder_tree(u32 sum[BUFFER])
{
    u32 final_sum = 0;
    const u32 mask = 0x1fffffff;
adder_tree:
    for (auto i = 0; i < BUFFER; i++)
    {
        final_sum += sum[i];
    }
    final_sum &= mask;
    return final_sum;
}

// Folding
u32 hls_vector_mul(const u32 *a, const u32 *b, const u32 *c)
{
    const u32 mask = 0x1fffffff;
    u32 sum[BUFFER];
    u32 final_sum;

init_sum:
	for (auto i = 0; i < BUFFER; i++)
    {
#pragma HLS UNROLL
        sum[i] = 0;
    }

calc:
	for (auto i = 0; i < N; i+=BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS UNROLL
            sum[j] += (c[i + j] + a[i + j] * b[i + j]) & mask;
        }
    }

    // TODO
    final_sum = adder_tree(sum);
    final_sum = final_sum*ALPHA;

    return final_sum;
}
