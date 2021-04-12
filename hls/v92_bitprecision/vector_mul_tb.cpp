#include <stdlib.h>
#include <stdio.h>
#include "vector_mul.h"

using namespace std;

u32 vector_mul(const u32 *a, const u32 *b, const u32 *c)
{
	u32 sum = 0;
    const u32 mask = 0x1fffffff;
    for (auto i = 0; i < N; i++)
    {
        sum += (a[i] * b[i] + c[i]) & mask;
    }
    sum &= mask;
    sum *= ALPHA;

    return sum;
}

u32 test_top(const u32 *a, const u32 *b, const u32 *c)
{
    hls::stream<trans_pkt> fifo_in;
    hls::stream<trans_pkt> fifo_out;
    u32 e_hls;
    //	Prepare FIFO
    trans_pkt in_tmp;
    for (auto i = 0; i < 3 * N; i++)
    {
        if (i < N)
        {
            in_tmp.data = a[i];
        }
        else
        {
            if (i < 2*N) in_tmp.data = b[i - N];
            else in_tmp.data = c[i - 2*N];
        }

        if (i == 3*N - 1)
        {
            in_tmp.last = 1;
        }
        else
        {
            in_tmp.last = 0;
        }

        in_tmp.keep = 0xf;
        in_tmp.strb = 0xf;

        fifo_in.write(in_tmp);
    }

    cout << "Computing ... " << endl;
    hls_vector_mul_top(fifo_in, fifo_out);

    in_tmp = fifo_out.read();
    e_hls = in_tmp.data;

    return e_hls;
}

int main()
{
    srand(0);

    u32 a[N], b[N], c[N], e_gold, e_hls;

    for (auto i = 0; i < N; i++)
    {
        a[i] = rand();
        b[i] = rand();
        c[i] = rand();
    }

    e_gold = vector_mul(a, b, c);
    e_hls = test_top(a, b, c);


    cout << "gold: " << e_gold << endl;
    cout << " HLS: " << e_hls << endl;

    if (e_gold != e_hls)
    {
        cout << "Error" << endl;
        return 1;
    }

    cout << "OK" << endl;

    return 0;
}
