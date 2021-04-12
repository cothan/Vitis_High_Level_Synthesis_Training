#include "vector_mul.h"

#define BUFFER 10

u29 adder_tree(u29 sum[BUFFER])
{
    u29 final_sum;
    u29 middle[5];
    u29 prev;

adder_tree:
    for (auto j = 0; j < BUFFER; j += 5)
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

void hls_vector_mul(const u29 a[BUFFER],
                    const u29 b[BUFFER],
                    const u29 c[BUFFER],
                    u29 sum[BUFFER], int i)
{
    u29 prev;
    for (auto j = 0; j < BUFFER; j++)
    {
#pragma HLS UNROLL
#pragma BIND_OP variable=a op=mul impl=dsp
#pragma BIND_OP variable=b op=mul impl=dsp
#pragma BIND_OP variable=c op=mul impl=dsp

        if (i == 0)
        {
            prev = static_cast<u29>(0);
        }
        else
        {
            prev = sum[j];
        }
        sum[j] = prev + (c[j] + a[j] * b[j]);
    }
}

u32 hls_reduce(u29 sum[BUFFER])
{
    u32 final_sum;

    final_sum = adder_tree(sum);
    final_sum = final_sum * ALPHA;

    return final_sum;
}

void hls_buffer(hls::stream<trans_pkt> &fifo_in,
                u29 a_buffer[BUFFER],
                u29 b_buffer[BUFFER],
                u29 c_buffer[BUFFER])
{
    trans_pkt tmp;
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

void hls_vector_mul_top(hls::stream<trans_pkt> &fifo_in,
                        hls::stream<trans_pkt> &fifo_out)
{
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return <-- Enable this make Cosim failed
#pragma HLS INTERFACE axis port=fifo_in depth=600
#pragma HLS INTERFACE axis port=fifo_out depth=1

    u32 final_sum;
    u29 a_buffer[BUFFER],
        b_buffer[BUFFER],
        c_buffer[BUFFER];
    u29 sum[BUFFER];
#pragma HLS ARRAY_RESHAPE variable=sum complete dim=1
#pragma HLS ARRAY_PARTITION variable=a_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=b_buffer complete dim=1
#pragma HLS ARRAY_PARTITION variable=c_buffer complete dim=1

    trans_pkt tmp, tmp_out;
    for (auto i = 0; i < 3*N; i+=3*BUFFER)
    {
    // Pipeline II=3*BUFFER
#pragma HLS PIPELINE II=30
        hls_buffer(fifo_in, a_buffer, b_buffer, c_buffer);
        hls_vector_mul(a_buffer, b_buffer, c_buffer, sum, i);
    }

    final_sum = hls_reduce(sum);

    // Vector Mul
    tmp_out.data = final_sum;
    tmp_out.last = 1;
    tmp_out.keep = 0xf;
    tmp_out.strb = 0xf;

    fifo_out.write(tmp_out);
}

// Total cycles: 611 cycles