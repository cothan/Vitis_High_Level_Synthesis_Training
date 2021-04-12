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
    e_hls = hls_vector_mul(a, b, c);

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
