            ; CALL XREF from entry0 @ 0x100003e74
            ;-- func.100003c60:
┌ 284: sym._neon_vector_mul (int64_t arg1, int64_t arg2, int64_t arg3);
│           ; arg int64_t arg1 @ x0
│           ; arg int64_t arg2 @ x1
│           ; arg int64_t arg3 @ x2
│           0x100003c60      00e4006f       movi v0.2d, 0000000000000000
│           0x100003c64      e8018092       movn x8, 0xf
│           0x100003c68      e90300aa       mov x9, x0                 ; arg1
│           0x100003c6c      ea0301aa       mov x10, x1                ; arg2
│           0x100003c70      eb0302aa       mov x11, x2                ; arg3
│           0x100003c74      01e4006f       movi v1.2d, 0000000000000000
│           0x100003c78      02e4006f       movi v2.2d, 0000000000000000
│           0x100003c7c      03e4006f       movi v3.2d, 0000000000000000
│       ┌─> 0x100003c80      2429df4c       ld1 {v4.4s, v5.4s, v6.4s, v7.4s}, [x9], 0x40
│       ╎   0x100003c84      5029df4c       ld1 {v16.4s, v17.4s, v18.4s, v19.4s}, [x10], 0x40
│       ╎   0x100003c88      7429df4c       ld1 {v20.4s, v21.4s, v22.4s, v23.4s}, [x11], 0x40
│       ╎   0x100003c8c      1496a44e       mla v20.4s, v16.4s, v4.4s
│       ╎   0x100003c90      3596a54e       mla v21.4s, v17.4s, v5.4s
│       ╎   0x100003c94      5696a64e       mla v22.4s, v18.4s, v6.4s
│       ╎   0x100003c98      7796a74e       mla v23.4s, v19.4s, v7.4s
│       ╎   0x100003c9c      1474076f       bic v20.4s, 0xe0, lsl 24
│       ╎   0x100003ca0      1574076f       bic v21.4s, 0xe0, lsl 24
│       ╎   0x100003ca4      1674076f       bic v22.4s, 0xe0, lsl 24
│       ╎   0x100003ca8      1774076f       bic v23.4s, 0xe0, lsl 24
│       ╎   0x100003cac      8386a34e       add v3.4s, v20.4s, v3.4s
│       ╎   0x100003cb0      a286a24e       add v2.4s, v21.4s, v2.4s
│       ╎   0x100003cb4      c186a14e       add v1.4s, v22.4s, v1.4s
│       ╎   0x100003cb8      e086a04e       add v0.4s, v23.4s, v0.4s
│       ╎   0x100003cbc      08410091       add x8, x8, 0x10
│       ╎   0x100003cc0      1fc102f1       cmp x8, 0xb0
│       └─< 0x100003cc4      e3fdff54       b.lo 0x100003c80
│           0x100003cc8      4284a34e       add v2.4s, v2.4s, v3.4s
│           0x100003ccc      4184a14e       add v1.4s, v2.4s, v1.4s
│           0x100003cd0      2084a04e       add v0.4s, v1.4s, v0.4s
│           0x100003cd4      00b8b14e       addv s0, v0.4s
│           0x100003cd8      0800261e       fmov w8, s0
│           0x100003cdc      090043b9       ldr w9, [x0, 0x300]        ; 0xc7 ; 199 ; arg1
│           0x100003ce0      2a0043b9       ldr w10, [x1, 0x300]       ; 0xc8 ; 200 ; arg2
│           0x100003ce4      4b0043b9       ldr w11, [x2, 0x300]       ; 0xc9 ; 201 ; arg3
│           0x100003ce8      492d091b       madd w9, w10, w9, w11
│           0x100003cec      29710012       and w9, w9, 0x1fffffff
│           0x100003cf0      2801080b       add w8, w9, w8
│           0x100003cf4      090443b9       ldr w9, [x0, 0x304]        ; 0xc7 ; 199 ; arg1
│           0x100003cf8      2a0443b9       ldr w10, [x1, 0x304]       ; 0xc8 ; 200 ; arg2
│           0x100003cfc      4b0443b9       ldr w11, [x2, 0x304]       ; 0xc9 ; 201 ; arg3
│           0x100003d00      492d091b       madd w9, w10, w9, w11
│           0x100003d04      2801080b       add w8, w9, w8
│           0x100003d08      090843b9       ldr w9, [x0, 0x308]        ; 0xc7 ; 199 ; arg1
│           0x100003d0c      2a0843b9       ldr w10, [x1, 0x308]       ; 0xc8 ; 200 ; arg2
│           0x100003d10      4b0843b9       ldr w11, [x2, 0x308]       ; 0xc9 ; 201 ; arg3
│           0x100003d14      492d091b       madd w9, w10, w9, w11
│           0x100003d18      2801080b       add w8, w9, w8
│           0x100003d1c      090c43b9       ldr w9, [x0, 0x30c]        ; 0xc7 ; 199 ; arg1
│           0x100003d20      2a0c43b9       ldr w10, [x1, 0x30c]       ; 0xc8 ; 200 ; arg2
│           0x100003d24      4b0c43b9       ldr w11, [x2, 0x30c]       ; 0xc9 ; 201 ; arg3
│           0x100003d28      492d091b       madd w9, w10, w9, w11
│           0x100003d2c      2801080b       add w8, w9, w8
│           0x100003d30      491043b9       ldr w9, [x2, 0x310]        ; 0xc9 ; 201 ; arg3
│           0x100003d34      00c4c03d       ldr q0, [x0, 0x310]        ; 0xc7 ; 199 ; arg1
│           0x100003d38      21c4c03d       ldr q1, [x1, 0x310]        ; 0xc8 ; 200 ; arg2
│           0x100003d3c      209ca04e       mul v0.4s, v1.4s, v0.4s
│           0x100003d40      4a1443b9       ldr w10, [x2, 0x314]       ; 0xc9 ; 201 ; arg3
│           0x100003d44      4b1843b9       ldr w11, [x2, 0x318]       ; 0xc9 ; 201 ; arg3
│           0x100003d48      4c1c43b9       ldr w12, [x2, 0x31c]       ; 0xc9 ; 201 ; arg3
│           0x100003d4c      00b8b14e       addv s0, v0.4s
│           0x100003d50      0d00261e       fmov w13, s0
│           0x100003d54      ac010c0b       add w12, w13, w12
│           0x100003d58      8b010b0b       add w11, w12, w11
│           0x100003d5c      6a010a0b       add w10, w11, w10
│           0x100003d60      4901090b       add w9, w10, w9
│           0x100003d64      2801080b       add w8, w9, w8
│           0x100003d68      08710012       and w8, w8, 0x1fffffff
│           0x100003d6c      29529352       movz w9, 0x9a91
│           0x100003d70      2929b572       movk w9, 0xa949, lsl 16
│           0x100003d74      007d091b       mul w0, w8, w9
└           0x100003d78      c0035fd6       ret
