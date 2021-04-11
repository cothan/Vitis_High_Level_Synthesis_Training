#include <arm_neon.h>
#include <stdlib.h>
#include <stdio.h>

#define vload(a, ptr) a = vld1q_u32_x4(ptr);
#define vmla(res, a, b, c) res = vmlaq_u32(a, b, c);
#define vadd(c, a, b) c = vaddq_u32(a, b);
#define vaddv(res, c) res = vaddvq_u32(c);
#define vand(c, a, b) c = vandq_u32(a, b);

#define N 200
#define ALPHA 2840173201 // 0b10101001010010011001101010010001

uint32_t neon_vector_mul(const uint32_t *a, const uint32_t *b, const uint32_t *c)
{
    // Total SIMD usage: 21
    uint32x4x4_t neon_a, neon_b, neon_c, neon_e, neon_tmp;
    uint32x4_t neon_mask;
    uint32_t sum_e[4], sum = 0;

    neon_e.val[0] = vdupq_n_u32(0);
    neon_e.val[1] = vdupq_n_u32(0);
    neon_e.val[2] = vdupq_n_u32(0);
    neon_e.val[3] = vdupq_n_u32(0);

    const uint32_t mask = 0x1fff1fff;
    neon_mask = vdupq_n_u32(mask);

    for (int i = 0; i < N; i += 4 * 4)
    {
        vload(neon_a, &a[i]);
        vload(neon_b, &b[i]);
        vload(neon_c, &c[i]);

        // tmp = c + a*b
        vmla(neon_tmp.val[0], neon_c.val[0], neon_a.val[0], neon_b.val[0]);
        vmla(neon_tmp.val[1], neon_c.val[1], neon_a.val[1], neon_b.val[1]);
        vmla(neon_tmp.val[2], neon_c.val[2], neon_a.val[2], neon_b.val[2]);
        vmla(neon_tmp.val[3], neon_c.val[3], neon_a.val[3], neon_b.val[3]);

        // tmp &= mask
        vand(neon_tmp.val[0], neon_tmp.val[0], neon_mask);
        vand(neon_tmp.val[1], neon_tmp.val[1], neon_mask);
        vand(neon_tmp.val[2], neon_tmp.val[2], neon_mask);
        vand(neon_tmp.val[3], neon_tmp.val[3], neon_mask);

        // e += tmp
        vadd(neon_e.val[0], neon_e.val[0], neon_tmp.val[0]);
        vadd(neon_e.val[1], neon_e.val[1], neon_tmp.val[1]);
        vadd(neon_e.val[2], neon_e.val[2], neon_tmp.val[2]);
        vadd(neon_e.val[3], neon_e.val[3], neon_tmp.val[3]);
    }

    vaddv(sum_e[0], neon_e.val[0]);
    vaddv(sum_e[1], neon_e.val[1]);
    vaddv(sum_e[2], neon_e.val[2]);
    vaddv(sum_e[3], neon_e.val[3]);

    sum += sum_e[0];
    sum += sum_e[1];
    sum += sum_e[2];
    sum += sum_e[3];

    sum &= mask;
    sum *= ALPHA;

    return sum;
}

uint32_t vector_mul(const uint32_t *a, const uint32_t *b, const uint32_t *c)
{
    uint32_t sum = 0;
    const uint32_t mask = 0x1fff1fff;
    for (int i = 0; i < N; i++)
    {
        sum += (c[i] + a[i] * b[i]) & mask;
    }
    sum &= mask;
    sum *= ALPHA;

    return sum;
}

int main()
{
    srand(0);

    uint32_t a[N], b[N], c[N], e_gold, e_neon;

    for (int i = 0; i < N; i++)
    {
        a[i] = rand();
        b[i] = rand();
        c[i] = rand();
    }

    e_gold = vector_mul(a, b, c);
    e_neon = neon_vector_mul(a, b, c);

    int ret = e_gold == e_neon;

    if (ret)
    {
        printf("Error\n");
        return 1;
    }

    printf("OK\n");

    return 0;
}
