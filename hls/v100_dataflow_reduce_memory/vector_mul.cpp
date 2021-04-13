#include "vector_mul.h"

// Reduce BUFFER size to 10
#define BUFFER 10

/* Pipeline
 * Pipeline to improve throughput
 */
u29 adder_tree(u29 sum[BUFFER])
{
    u29 final_sum = 0;
    u29 middle[5];

adder_tree:
	for (auto j = 0; j < BUFFER; j+=5)
    {
#pragma HLS PIPELINE II=1
		for (auto i = 0; i < 5; i++)
        {
#pragma HLS UNROLL
            auto prev = (j == 0) ? static_cast<u29>(0) : middle[i];
            middle[i] = prev + sum[j + i];
        }
    }

reduce:
    for (auto i = 0; i < 5; i++)
    {
#pragma HLS UNROLL
        final_sum += middle[i];
    }

    return final_sum;
}

void hls_buffer(hls::stream<trans_pkt> &fifo_in,
                u29 a_buffer[BUFFER],
                u29 b_buffer[BUFFER],
                u29 c_buffer[BUFFER])
{
    trans_pkt tmp;
buffering:
    for (auto k = 0; k < 3*BUFFER; k++)
    {
        tmp = fifo_in.read();

        auto index = k;
        switch ( index % 3)
        {
        case 0:
            a_buffer[index / 3 % BUFFER] = tmp.data;
            break;
        case 1:
            b_buffer[index / 3 % BUFFER] = tmp.data;
            break;
        default:
            c_buffer[index / 3 % BUFFER] = tmp.data;
            break;
        }
    }
}


u32 hls_reduce(u29 sum[BUFFER])
{
    u32 final_sum;

    final_sum = adder_tree(sum);
    final_sum = final_sum * ALPHA;

    return final_sum;
}


/* Bit precision
 * Remove masking, after synthesis, we see huge resources usage drop
 */
void hls_vector_mul_part(const u29 a[BUFFER],
                        const u29 b[BUFFER],
                        const u29 c[BUFFER],
                        u29 sum[BUFFER], int i)
{
calc:
    for (auto j = 0; j < BUFFER; j++)
    {
#pragma HLS UNROLL
#pragma BIND_OP variable=a op=mul impl=dsp
#pragma BIND_OP variable=b op=mul impl=dsp
#pragma BIND_OP variable=c op=add impl=dsp
        auto prev = (i == 0) ? static_cast<u29>(0) : sum[j];
        sum[j] = prev + (c[j] + a[j] * b[j]);
    }
}

/* Block level IO interface: AXIS (FIFO)
 * New assumption data flow: A|B|C -> A|B|C
 * Refactor out data flow
 */
void hls_vector_mul_top(hls::stream<trans_pkt> &fifo_in,
						hls::stream<trans_pkt> &fifo_out)
{
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return <-- Enable this make Cosim failed
#pragma HLS INTERFACE axis port=fifo_in  depth=600
#pragma HLS INTERFACE axis port=fifo_out depth=1

    u32 final_sum;
    static u29  a_buffer[BUFFER],
		        b_buffer[BUFFER],
		        c_buffer[BUFFER],
                sum[BUFFER];
    trans_pkt tmp, tmp_out;
#pragma HLS ARRAY_PARTITION variable=a_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=c_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=sum complete dim=1

dataflow:
for (auto i = 0; i < 3*N; i+=3*BUFFER)
    {
    // Pipeline II=3*BUFFER
#pragma HLS PIPELINE II=30
        hls_buffer(fifo_in, a_buffer, b_buffer, c_buffer);
        hls_vector_mul_part(a_buffer, b_buffer, c_buffer, sum, i);
    }
    final_sum = hls_reduce(sum);

    // Vector Mul
    tmp_out.data = final_sum;
	tmp_out.last = 1;
	tmp_out.keep = 0xf;
	tmp_out.strb = 0xf;

	fifo_out.write(tmp_out);
}
