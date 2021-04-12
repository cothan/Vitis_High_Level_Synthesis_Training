#include "vector_mul.h"

void hls_buffer(hls::stream<trans_pkt> &fifo_in,
                u29 *a_buffer,
                u29 *b_buffer,
                u29 *c_buffer)
{
    trans_pkt tmp;
    for (auto k = 0; k < 3; k++)
    {
        tmp = fifo_in.read();

        auto index = k;
        switch (index )
        {
        case 0:
            *a_buffer = tmp.data;
            break;
        case 1:
            *b_buffer = tmp.data;
            break;
        default:
            *c_buffer = tmp.data;
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
    u29 a_buffer,
        b_buffer,
        c_buffer;
    u29 sum = 0;
    
    trans_pkt tmp, tmp_out;
    for (auto i = 0; i < 3*N; i+=3)
    {
    // Pipeline II=3*BUFFER
#pragma HLS PIPELINE II=3
        hls_buffer(fifo_in, &a_buffer, &b_buffer, &c_buffer);
        sum += c_buffer + a_buffer*b_buffer;
    }

    final_sum = sum * ALPHA;

    // Vector Mul
    tmp_out.data = final_sum;
    tmp_out.last = 1;
    tmp_out.keep = 0xf;
    tmp_out.strb = 0xf;

    fifo_out.write(tmp_out);
}
