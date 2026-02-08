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
    uint64_t ra;
    uint64_t sp;
    uint64_t gp;
    uint64_t tp;
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t s0;
    uint64_t s1;
    uint64_t a0;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    uint64_t t6;
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
