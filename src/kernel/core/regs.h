#pragma once

#define read_csr(csr)                                 \
    ({                                                \
        size_t __tmp;                                 \
        asm volatile("csrr %0, " #csr : "=r"(__tmp)); \
        __tmp;                                        \
    })

#define write_csr$(reg, value)                                  \
    do {                                                        \
        uint32_t __tmp = (value);                               \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
    } while (0)
