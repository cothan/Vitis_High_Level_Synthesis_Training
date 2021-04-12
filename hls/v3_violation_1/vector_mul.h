#ifndef VECTOR_MUL_H
#define VECTOR_MUL_H

#include "ap_int.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"

#define N 200
#define SIZE N

#define ALPHA 2840173201 // 0b10101001010010011001101010010001


#define DWIDTH 32

typedef ap_uint<8> u8;
typedef ap_uint<29> u29;
typedef ap_uint<32> u32;
typedef ap_axiu<DWIDTH, 0, 0, 0> trans_pkt;

u32 hls_vector_mul(const u32 a[N], const u32 b[N], const u32 c[N]);

void hls_vector_mul_top(hls::stream<trans_pkt> &fifo_in,
						hls::stream<trans_pkt> &fifo_out);

#endif
