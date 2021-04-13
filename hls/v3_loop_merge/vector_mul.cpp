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
u32 hls_vector_mul(const u32 a[N], const u32 b[N], const u32 c[N])
{
    const u32 mask = 0x1fffffff;
    u32 sum[BUFFER];
    u32 final_sum;


calc:
	for (auto i = 0; i < N; i+=BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS UNROLL
            if (i == 0)
            {
                sum[j] = static_cast<u32>(0);
            }
            else{
                sum[j] = sum[j];
            }
            sum[j] += (c[i + j] + a[i + j] * b[i + j]) & mask;
        }
    }

    // TODO
    final_sum = adder_tree(sum);
    final_sum = final_sum*ALPHA;

    return final_sum;
}
