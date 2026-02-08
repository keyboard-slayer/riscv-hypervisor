#include <logger.h>
#include <stddef.h>

#include "exception.h"
#include "vcpu.h"

[[gnu::section(".text.stvec"), gnu::naked, gnu::aligned(8)]] void trap_handler(void) {
    __asm__ volatile(
        "csrrw a0, sscratch, a0\n"

        "sd ra, %[ra_offset](a0)\n"
        "sd sp, %[sp_offset](a0)\n"
        "sd gp, %[gp_offset](a0)\n"
        "sd tp, %[tp_offset](a0)\n"
        "sd t0, %[t0_offset](a0)\n"
        "sd t1, %[t1_offset](a0)\n"
        "sd t2, %[t2_offset](a0)\n"
        "sd s0, %[s0_offset](a0)\n"
        "sd s1, %[s1_offset](a0)\n"
        "sd a1, %[a1_offset](a0)\n"
        "sd a2, %[a2_offset](a0)\n"
        "sd a3, %[a3_offset](a0)\n"
        "sd a4, %[a4_offset](a0)\n"
        "sd a5, %[a5_offset](a0)\n"
        "sd a6, %[a6_offset](a0)\n"
        "sd a7, %[a7_offset](a0)\n"
        "sd s2, %[s2_offset](a0)\n"
        "sd s3, %[s3_offset](a0)\n"
        "sd s4, %[s4_offset](a0)\n"
        "sd s5, %[s5_offset](a0)\n"
        "sd s6, %[s6_offset](a0)\n"
        "sd s7, %[s7_offset](a0)\n"
        "sd s8, %[s8_offset](a0)\n"
        "sd s9, %[s9_offset](a0)\n"
        "sd s10, %[s10_offset](a0)\n"
        "sd s11, %[s11_offset](a0)\n"
        "sd t3, %[t3_offset](a0)\n"
        "sd t4, %[t4_offset](a0)\n"
        "sd t5, %[t5_offset](a0)\n"
        "sd t6, %[t6_offset](a0)\n"

        "csrr t0, sscratch\n"
        "sd t0, %[a0_offset](a0)\n"
        "ld sp, %[host_sp_offset](a0)\n"

        "call virt_exception_handler\n"
        :
        : [ra_offset] "i"(offsetof(Vcpu, frame.ra)),
          [sp_offset] "i"(offsetof(Vcpu, frame.sp)),
          [gp_offset] "i"(offsetof(Vcpu, frame.gp)),
          [tp_offset] "i"(offsetof(Vcpu, frame.tp)),
          [t0_offset] "i"(offsetof(Vcpu, frame.t0)),
          [t1_offset] "i"(offsetof(Vcpu, frame.t1)),
          [t2_offset] "i"(offsetof(Vcpu, frame.t2)),
          [s0_offset] "i"(offsetof(Vcpu, frame.s0)),
          [s1_offset] "i"(offsetof(Vcpu, frame.s1)),
          [a0_offset] "i"(offsetof(Vcpu, frame.a0)),
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
          [host_sp_offset] "i"(offsetof(Vcpu, host_sp))
    );
}

static char *exceptions[] = {
    "Instruction address misaligned",
    "Instruction access fault",
    "Illegal instruction",
    "Breakpoint",
    "Load address misaligned",
    "Load access fault",
    "Store/AMO address misaligned",
    "Store/AMO access fault",
    "Environment call from U/VU-mode",
    "Environment call from HS-mode",
    "Environment call from VS-mode",
    "Environment call from M-mode",
    "Instruction page fault",
    "Load page fault",
    "Store/AMO page fault",
    "Instruction guest-page fault",
    "Load guest-page fault",
    "Virtual instruction",
    "Store/AMO guest-page fault",
    [20] = "Instruction guest-page fault",
    [21] = "Load guest-page fault",
    [22] = "Virtual instruction",
    [23] = "Store/AMO guest-page fault",
};

static char const *interrupts[] = {
    "User software interrupt",
    "Supervisor software interrupt",
    "Hypervisor software interrupt",
    "Machine software interrupt",
    "User timer interrupt",
    "Supervisor timer interrupt",
    "Hypervisor timer interrupt",
    "Machine timer interrupt",
    "User external interrupt",
    "Supervisor external interrupt",
    "Hypervisor external interrupt",
    "Machine external interrupt",
};

static void panic_handler(Stackframe *frame, uint32_t scause, uint32_t stval, uint32_t user_pc) {
    print$("\n\x1b[0;31m!!! \x1b[33m---------------------------------------------------------------------------------------------------\x1b[0m\n\n");
    print$("    KERNEL PANIC\n\n");
    print$("    %s was raised\n", exceptions[scause]);
    print$("    scause: %d, stval: %x\n\n", scause, stval);
    print$("    RA   %016x GP  %016x TP  %016x SP %016x\n", frame->ra, frame->gp, frame->sp);
    print$("    T0   %016x T1  %016x T2  %016x T3 %016x\n", frame->t0, frame->t1, frame->t2, frame->t3);
    print$("    T4   %016x T5  %016x T6  %016x A0 %016x\n", frame->t4, frame->t5, frame->t6, frame->a0);
    print$("    A1   %016x A2  %016x A3  %016x A4 %016x\n", frame->a1, frame->a2, frame->a3, frame->a4);
    print$("    A5   %016x A6  %016x A7  %016x S0 %016x\n", frame->a5, frame->a6, frame->a7, frame->s0);
    print$("    S1   %016x S2  %016x S3  %016x S4 %016x\n", frame->s1, frame->s2, frame->s3, frame->s4);
    print$("    S5   %016x S6  %016x S7  %016x S8 %016x\n", frame->s5, frame->s6, frame->s7, frame->s8);
    print$("    S9   %016x S10 %016x S11 %016x\n", frame->s9, frame->s10, frame->s11);
    print$("    SEPC \x1b[7m%016x\x1b[0m\n", user_pc);
    print$("\n\x1b[33m--------------------------------------------------------------------------------------------------- \x1b[0;31m!!!\x1b[0m\n\n");

    for (;;) {
        __asm__ volatile("wfi");
    }
}

void register_handler(void) {
    write_csr$(stvec, (size_t)interrupt_kernel);
}

void virt_exception_handler(Vcpu *vcpu) {
    uint64_t scause = read_csr$(scause);
    uint64_t stval = read_csr$(stval);
    uint64_t user_pc = read_csr$(sepc);

    if (scause & (1 << 31))
        log$("Interrupt: %s", interrupts[scause & 0xff]);
    else if (scause == 10) {
        log$("SBI Call: eid=0x%x fid=0x%x a0=0x%x (%c)", vcpu->frame.a7, vcpu->frame.a6, vcpu->frame.a0, vcpu->frame.a0);
        vcpu->sepc = user_pc + 4;
    } else
        panic_handler(&vcpu->frame, scause, stval, user_pc);

    cpu_run(vcpu);
}

Stackframe *exception_handler(Stackframe *frame) {
    uint64_t scause = read_csr$(scause);
    uint64_t stval = read_csr$(stval);
    uint64_t user_pc = read_csr$(sepc);

    if (scause & (1 << 31))
        log$("Interrupt: %s", interrupts[scause & 0xff]);
    else
        panic_handler(frame, scause, stval, user_pc);

    return frame;
}
