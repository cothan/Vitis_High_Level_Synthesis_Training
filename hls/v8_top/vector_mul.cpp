#include "vector_mul.h"

#define BUFFER 10


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
            auto prev = (i == 0) ? static_cast<u32>(0) : middle[i];
            middle[i] = prev + sum[j + i];
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
#pragma BIND_OP variable=a op=mul impl=dsp
#pragma BIND_OP variable=b op=mul impl=dsp
#pragma BIND_OP variable=c op=mul impl=dsp
            auto prev = (i == 0) ? static_cast<u32>(0) : sum[j];
            sum[j] = prev + (c[i/BUFFER][j] + a[i/BUFFER][j] * b[i/BUFFER][j]) & mask;
        }
    }

    // TODO
    final_sum = adder_tree(sum);
    final_sum = final_sum*ALPHA;

    return final_sum;
}

void hls_vector_mul_top(hls::stream<trans_pkt> &fifo_in,
						hls::stream<trans_pkt> &fifo_out)
{
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return <-- Enable this make Cosim failed
#pragma HLS INTERFACE axis port=fifo_in  depth=600
#pragma HLS INTERFACE axis port=fifo_out depth=1

    u32 final_sum;
    static u32  a_buffer[N/BUFFER][BUFFER],
		        b_buffer[N/BUFFER][BUFFER],
		        c_buffer[N/BUFFER][BUFFER];
    trans_pkt tmp, tmp_out;
#pragma HLS ARRAY_PARTITION variable=a_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=c_buffer complete dim=2

buffering:
    for (auto i = 0; i < 3*N; i+= BUFFER)
    {
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE II=1
        	tmp = fifo_in.read();
        	if (i < N) {
        		a_buffer[i/BUFFER][j] = tmp.data;
        	}
        	else
        	{
        		if (i < 2*N) b_buffer[(i-N)/BUFFER][j] = tmp.data;
        		else c_buffer[(i-2*N)/BUFFER][j] = tmp.data;
        	}
        }
    }
    final_sum = hls_vector_mul_part(a_buffer, b_buffer, c_buffer);

    // Vector Mul
    tmp_out.data = final_sum;
	tmp_out.last = 1;
	tmp_out.keep = 0xf;
	tmp_out.strb = 0xf;

	fifo_out.write(tmp_out);
}





