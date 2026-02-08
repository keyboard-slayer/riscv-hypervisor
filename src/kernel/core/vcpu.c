#include <errno.h>
#include <unistd.h>

#include "exception.h"
#include "paging.h"
#include "pmm.h"
#include "regs.h"
#include "vcpu.h"

int cpu_init(Vcpu *self, uintptr_t *page, uintptr_t sepc) {
    PhysObj stack = pmm_alloc(STACK_SIZE / getpagesize());
    if (stack.len == 0)
        return -ENOMEM;

    self->host_sp = stack.base + stack.len;
    self->sstatus = 1UL << 8;
    self->hgatp = hgatp$(page);
    self->sepc = sepc;

    Hstatus hstatus = {0};
    hstatus.vsxl = 2;
    hstatus.spv = 1;
    self->hstatus = hstatus.bits;

    return 0;
}

void cpu_run(Vcpu *self) {
    __asm__ volatile(
        "csrw hstatus, %[hstatus]\n"
        "csrw sstatus, %[sstatus]\n"
        "csrw sscratch, %[vcpu]\n"
        "csrw hgatp, %[hgatp]\n"
        "csrw sepc, %[sepc]\n"
        "csrw stvec, %[stvec]\n"

        "mv a0, %[vcpu]\n"
        "ld ra, %[ra_offset](a0)\n"
        "ld sp, %[sp_offset](a0)\n"
        "ld gp, %[gp_offset](a0)\n"
        "ld tp, %[tp_offset](a0)\n"
        "ld t0, %[t0_offset](a0)\n"
        "ld t1, %[t1_offset](a0)\n"
        "ld t2, %[t2_offset](a0)\n"
        "ld s0, %[s0_offset](a0)\n"
        "ld s1, %[s1_offset](a0)\n"
        "ld a1, %[a1_offset](a0)\n"
        "ld a2, %[a2_offset](a0)\n"
        "ld a3, %[a3_offset](a0)\n"
        "ld a4, %[a4_offset](a0)\n"
        "ld a5, %[a5_offset](a0)\n"
        "ld a6, %[a6_offset](a0)\n"
        "ld a7, %[a7_offset](a0)\n"
        "ld s2, %[s2_offset](a0)\n"
        "ld s3, %[s3_offset](a0)\n"
        "ld s4, %[s4_offset](a0)\n"
        "ld s5, %[s5_offset](a0)\n"
        "ld s6, %[s6_offset](a0)\n"
        "ld s7, %[s7_offset](a0)\n"
        "ld s8, %[s8_offset](a0)\n"
        "ld s9, %[s9_offset](a0)\n"
        "ld s10, %[s10_offset](a0)\n"
        "ld s11, %[s11_offset](a0)\n"
        "ld t3, %[t3_offset](a0)\n"
        "ld t4, %[t4_offset](a0)\n"
        "ld t5, %[t5_offset](a0)\n"
        "ld t6, %[t6_offset](a0)\n"
        "ld a0, %[a0_offset](a0)\n"

        "sret"
        :
        : [hstatus] "r"(self->hstatus),
          [sstatus] "r"(self->sstatus),
          [vcpu] "r"((uintptr_t)self),
          [hgatp] "r"(self->hgatp),
          [sepc] "r"(self->sepc),
          [stvec] "r"((uintptr_t)trap_handler),
          [ra_offset] "i"(offsetof(Vcpu, frame.ra)),
          [sp_offset] "i"(offsetof(Vcpu, frame.sp)),
          [gp_offset] "i"(offsetof(Vcpu, frame.gp)),
          [tp_offset] "i"(offsetof(Vcpu, frame.tp)),
          [t0_offset] "i"(offsetof(Vcpu, frame.t0)),
          [t1_offset] "i"(offsetof(Vcpu, frame.t1)),
          [t2_offset] "i"(offsetof(Vcpu, frame.t2)),
          [s0_offset] "i"(offsetof(Vcpu, frame.s0)),
          [s1_offset] "i"(offsetof(Vcpu, frame.s1)),
          [a1_offset] "i"(offsetof(Vcpu, frame.a1)),
          [a2_offset] "i"(offsetof(Vcpu, frame.a2)),
          [a3_offset] "i"(offsetof(Vcpu, frame.a3)),
          [a4_offset] "i"(offsetof(Vcpu, frame.a4)),
          [a5_offset] "i"(offsetof(Vcpu, frame.a5)),
          [a6_offset] "i"(offsetof(Vcpu, frame.a6)),
          [a7_offset] "i"(offsetof(Vcpu, frame.a7)),
          [s2_offset] "i"(offsetof(Vcpu, frame.s2)),
          [s3_offset] "i"(offsetof(Vcpu, frame.s3)),
          [s4_offset] "i"(offsetof(Vcpu, frame.s4)),
          [s5_offset] "i"(offsetof(Vcpu, frame.s5)),
          [s6_offset] "i"(offsetof(Vcpu, frame.s6)),
          [s7_offset] "i"(offsetof(Vcpu, frame.s7)),
          [s8_offset] "i"(offsetof(Vcpu, frame.s8)),
          [s9_offset] "i"(offsetof(Vcpu, frame.s9)),
          [s10_offset] "i"(offsetof(Vcpu, frame.s10)),
          [s11_offset] "i"(offsetof(Vcpu, frame.s11)),
          [t3_offset] "i"(offsetof(Vcpu, frame.t3)),
          [t4_offset] "i"(offsetof(Vcpu, frame.t4)),
          [t5_offset] "i"(offsetof(Vcpu, frame.t5)),
          [t6_offset] "i"(offsetof(Vcpu, frame.t6)),
          [a0_offset] "i"(offsetof(Vcpu, frame.a0))
    );
}
