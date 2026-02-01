#include "sbi.h"

SbiReturn __ecall_impl(size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t fid, size_t eid) {
    register size_t a0 __asm__("a0") = arg0;
    register size_t a1 __asm__("a1") = arg1;
    register size_t a2 __asm__("a2") = arg2;
    register size_t a3 __asm__("a3") = arg3;
    register size_t a4 __asm__("a4") = arg4;
    register size_t a5 __asm__("a5") = arg5;
    register size_t a6 __asm__("a6") = fid;
    register size_t a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");
    return (SbiReturn){.err = a0, .value = a1};
}
