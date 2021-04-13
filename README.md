## High Level Synthesis Introduction

### Summary

This is my guest lecture from ECE 615 2021 at George Mason Univerisy. 

This lecture consist of demo, live coding, and slides based on a self-made algorithm.


### Problems and Correctness

Algorithm
```c
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
```


The proposed algorithm in this lecture is not a practical algorithm, I mix popular coding pattern I've seen from image processing, cryptography, high performance computing. 

All code in this repo are verified, passed `C simulation` and `Co-simulation`. 

### Director tree 

```
.
├── Algo_Vitis_High_Level_Synthesis_Introduction.pdf
├── README.adoc
├── Slides_Vitis_High_Level_Synthesis_Introduction.pdf
├── hls
│   ├── v0_baseline
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v1_unroll
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v2_folding
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v3_loop_merge
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v3_violation_1
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v4_parition
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v5_adder_tree
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v5_loop_merge
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v5_violation
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v6_adjust_buffer
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v7_bindop
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v8_top
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├──v9_flatten
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v91_pipeline
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v92_bitprecision
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├── v92_dataflow
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   ├──v100_dataflow_reduce_memory
│   │   ├── vector_mul.cpp
│   │   ├── vector_mul.h
│   │   └── vector_mul_tb.cpp
│   └── v101_streaming
│       ├── vector_mul.cpp
│       ├── vector_mul.h
│       └── vector_mul_tb.cpp
|
└── neon
    ├── neon_vector_mul_r2.asm
    └── vector_mul.c
```

### Contribution

Knowledge from this work is taken from public domain, thus, returned to public domain. Feel free to make a pull request. 


### Contact 

Feel free to shoot me an email, if you do NEON versus HLS comparison. 

