#pragma once

#include <stddef.h>
#include <stdint.h>

#define read_csr$(reg)                                        \
    ({                                                        \
        size_t __tmp;                                         \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                                \
    })

#define write_csr$(reg, value)                                  \
    do {                                                        \
        size_t __tmp = (value);                                 \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
    } while (0)

typedef struct __attribute__((packed)) {
    size_t ra;
    size_t gp;
    size_t tp;
    size_t t0;
    size_t t1;
    size_t t2;
    size_t t3;
    size_t t4;
    size_t t5;
    size_t t6;
    size_t a0;
    size_t a1;
    size_t a2;
    size_t a3;
    size_t a4;
    size_t a5;
    size_t a6;
    size_t a7;
    size_t s0;
    size_t s1;
    size_t s2;
    size_t s3;
    size_t s4;
    size_t s5;
    size_t s6;
    size_t s7;
    size_t s8;
    size_t s9;
    size_t s10;
    size_t s11;
    size_t sp;
} Stackframe;

typedef union {
    struct {
        uint64_t wpri_0 : 5;
        uint64_t vasbe : 1;
        uint64_t gva : 1;
        uint64_t spv : 1;
        uint64_t spvp : 1;
        uint64_t hu : 1;
        uint64_t wpri_1 : 2;
        uint64_t vgein : 6;
        uint64_t wpri_2 : 2;
        uint64_t vtvm : 1;
        uint64_t vtw : 1;
        uint64_t vtsr : 1;
        uint64_t wpri_3 : 9;
        uint64_t vsxl : 2;
        uint64_t wpri_4 : 14;
        uint64_t hupmm : 2;
        uint64_t wpri_5 : 14;
    };

    uint64_t bits;
} Hstatus;
