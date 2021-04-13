#include "vector_mul.h"

#define BUFFER 20


u32 adder_tree(u32 sum[BUFFER])
{
    u32 middle[5];
    const u32 mask = 0x1fffffff;
    u32 final_sum = 0;

adder_tree:
	for (auto j = 0; j < BUFFER; j+=5)
    {
		for (auto i = 0; i < 5; i++)
        {
#pragma HLS UNROLL
            if (j == 0)
            {
                middle[i] = 0;
            }
            else
            {
                middle[i] = middle[i];
            }
            middle[i] += sum[j + i];
        }
    }

reduce:
    for (auto i = 0; i < 5; i++)
    {
#pragma HLS UNROLL
        final_sum += middle[i];
    }

    final_sum &= mask;
    return final_sum;
}

// Solve Partition
u32 hls_vector_mul_part(const u32 a[N/BUFFER][BUFFER],
                        const u32 b[N/BUFFER][BUFFER],
                        const u32 c[N/BUFFER][BUFFER])
{
    const u32 mask = 0x1fffffff;

    u32 final_sum;
    u32 sum[BUFFER];
#pragma HLS ARRAY_RESHAPE variable=sum complete dim=1


calc:
	for (auto i = 0; i < N; i+=BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS UNROLL
            auto prev = (i == 0) ? static_cast<u32>(0) : sum[j];
            sum[j] = prev + (c[i/BUFFER][j] + a[i/BUFFER][j] * b[i/BUFFER][j]) & mask;
        }
    }

    // TODO
    final_sum = adder_tree(sum);
    final_sum = final_sum*ALPHA;

    return final_sum;
}

u32 hls_vector_mul(const u32 a[N], const u32 b[N], const u32 c[N])
{
    u32 final_sum;
    u32 a_buffer[N/BUFFER][BUFFER],
		b_buffer[N/BUFFER][BUFFER],
		c_buffer[N/BUFFER][BUFFER];
#pragma HLS ARRAY_PARTITION variable=a_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=c_buffer complete dim=2

buffering:
    for (auto i = 0; i < N; i+= BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS PIPELINE II=1
            a_buffer[i/BUFFER][j] = a[i+j];
            b_buffer[i/BUFFER][j] = b[i+j];
            c_buffer[i/BUFFER][j] = c[i+j];
        }
    }
    final_sum = hls_vector_mul_part(a_buffer, b_buffer, c_buffer);

    return final_sum;
}





