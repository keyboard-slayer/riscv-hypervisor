#pragma once

#define read_csr(csr)                                 \
    ({                                                \
        size_t __tmp;                                 \
        asm volatile("csrr %0, " #csr : "=r"(__tmp)); \
        __tmp;                                        \
    })

#define HIGH_BIT 0x8000000000000000ULL

void register_handler(void);
