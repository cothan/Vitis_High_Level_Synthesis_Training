#include "vector_mul.h"


void hls_vector_mul_top(hls::stream<trans_pkt> &fifo_in,
						hls::stream<trans_pkt> &fifo_out)
{
#pragma HLS INTERFACE s_axilite port=return
//#pragma HLS INTERFACE ap_ctrl_none port=return <-- Enable this make Cosim failed
#pragma HLS INTERFACE axis port=fifo_in  depth=400
#pragma HLS INTERFACE axis port=fifo_out depth=1

    static u32 RAM_A[SIZE];
    static u32 RAM_B[SIZE];
    static u32 RAM_C[SIZE];

    trans_pkt tmp, tmp_out;

fifo_AB:
    for (auto i = 0; i < SIZE * 2; i++)
    {
#pragma HLS PIPELINE II=1
        tmp = fifo_in.read();

        if (i < SIZE)
        {
            RAM_A[i] = tmp.data;
        }
        else
        {
            RAM_B[i - SIZE] = tmp.data;
        }
    }

    // Vector Mul
    tmp_out.data = hls_vector_mul(RAM_A, RAM_B, RAM_C);

	tmp_out.last = 1;
	tmp_out.keep = 0xf;
	tmp_out.strb = 0xf;

	fifo_out.write(tmp_out);
}
