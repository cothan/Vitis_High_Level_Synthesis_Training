#include "vector_mul.h"

#define BUFFER 10


u29 adder_tree(u29 sum[BUFFER])
{
    u29 final_sum;
    u29 middle[5];
    u29 prev;

adder_tree:
	for (auto j = 0; j < BUFFER; j+=5)
    {
		for (auto i = 0; i < 5; i++)
        {
#pragma HLS LOOP_FLATTEN
#pragma HLS UNROLL
            if (j == 0)
            {
                prev = 0;
            }
            else
            {
                prev = middle[i];
            }
            middle[i] = prev + sum[j + i];
        }
    }

reduce:
    for (auto i = 0; i < 5; i++)
    {
#pragma HLS UNROLL
    	if (i == 0)
    	{
    		prev = 0;
    	}
    	else
    	{
    		prev = final_sum;
    	}
        final_sum = prev + middle[i];
    }

    return final_sum;
}


// Solve Partition
u32 hls_vector_mul_part(const u29 a[N/BUFFER][BUFFER],
                        const u29 b[N/BUFFER][BUFFER],
                        const u29 c[N/BUFFER][BUFFER])
{
    u32 final_sum;
    u29 prev;
    u29 sum[BUFFER];
#pragma HLS ARRAY_RESHAPE variable=sum complete dim=1


calc:
	for (auto i = 0; i < N; i+=BUFFER)
    {
#pragma HLS PIPELINE II=1
        for (auto j = 0; j < BUFFER; j++)
        {
#pragma HLS UNROLL
#pragma BIND_OP variable=a op=mul impl=dsp

            if (i == 0)
            {
                prev = static_cast<u29>(0);
            }
            else{
                prev = sum[j];
            }
            sum[j] = prev + (c[i/BUFFER][j] + a[i/BUFFER][j] * b[i/BUFFER][j]);
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
    u29 a_buffer[N/BUFFER][BUFFER],
		b_buffer[N/BUFFER][BUFFER],
		c_buffer[N/BUFFER][BUFFER];
    trans_pkt tmp, tmp_out;
#pragma HLS ARRAY_PARTITION variable=a_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=2
#pragma HLS ARRAY_PARTITION variable=c_buffer complete dim=2

#pragma HLS DATAFLOW

buffering:
    for (auto i = 0; i < 3*N; i++)
    {
#pragma HLS LOOP_FLATTEN
#pragma HLS PIPELINE II=1
        tmp = fifo_in.read();

        // if (i < N) {
        // 	a_buffer[i/BUFFER][j] = tmp.data;
        // }
        // else
        // {
        // 	if (i < 2*N) b_buffer[(i-N)/BUFFER][j] = tmp.data;
        // 	else c_buffer[(i-2*N)/BUFFER][j] = tmp.data;
        // }

        switch (i % 3)
        {
        case 0:
            a_buffer[i/(3*BUFFER)][i/3 % BUFFER] = tmp.data;
            break;
        case 1:
            b_buffer[i/(3*BUFFER)][i/3 % BUFFER] = tmp.data;
            break;
        default:
            c_buffer[i/(3*BUFFER)][i/3 % BUFFER] = tmp.data;
            break;
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






