#include <arm_neon.h>
#include <stdlib.h>
#include <stdio.h>

/* Compile flag
 * gcc vector_mul.c -o out -O3 -fno-tree-vectorize -g3 -Wall -Wextra -Wpedantic
 */

#define vload(a, ptr) a = vld1q_u32_x4(ptr);
#define vstore(ptr, a) vst1q_u32_x4(ptr, a);
#define vmla(res, a, b, c) res = vmlaq_u32(a, b, c);
#define vadd(c, a, b) c = vaddq_u32(a, b);
#define vaddv(res, c) res = vaddvq_u32(c);
#define vand(c, a, b) c = vandq_u32(a, b);

#define N 400
#define ALPHA 2840173201 // 0b10101001010010011001101010010001

void print_vectors(uint32x4x4_t v)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%8x - ", v.val[i][j]);
        }
        printf("\n");
    }
}

uint32_t neon_vector_mul(const uint32_t *a, const uint32_t *b, const uint32_t *c)
{
    // Total SIMD usage: 21
    uint32x4x4_t neon_a, neon_b, neon_c, neon_e, neon_tmp; // 20
    uint32x4_t neon_mask;                                  // 1
    uint32_t sum;

    neon_e.val[0] = vdupq_n_u32(0);
    neon_e.val[1] = vdupq_n_u32(0);
    neon_e.val[2] = vdupq_n_u32(0);
    neon_e.val[3] = vdupq_n_u32(0);

    const uint32_t mask = 0x1fffffff;
    neon_mask = vdupq_n_u32(mask);

    for (int i = 0; i < N; i += 16)
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
    // e += tmp
    vadd(neon_e.val[0], neon_e.val[0], neon_e.val[1]);
    vadd(neon_e.val[0], neon_e.val[0], neon_e.val[2]);
    vadd(neon_e.val[0], neon_e.val[0], neon_e.val[3]);

    vaddv(sum, neon_e.val[0]);

    sum &= mask;
    sum *= ALPHA;

    return sum;
}

uint32_t vector_mul(const uint32_t *a, const uint32_t *b, const uint32_t *c)
{
    uint32_t sum = 0;
    const uint32_t mask = 0x1fffffff;
    for (int i = 0; i < N; i++)
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

    uint32_t a[N], b[N], c[N], e_gold, e_neon;

    for (int i = 0; i < N; i++)
    {
        a[i] = rand();
        b[i] = rand();
        c[i] = rand();
    }

    e_gold = vector_mul(a, b, c);
    e_neon = neon_vector_mul(a, b, c);

    printf("%d -- %d\n", e_gold, e_neon);

    if (e_gold != e_neon)
    {
        printf("Error\n");
        return 1;
    }

    printf("OK\n");

    return 0;
}
