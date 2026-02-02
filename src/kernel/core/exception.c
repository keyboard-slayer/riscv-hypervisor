#include <logger.h>

#include "exception.h"

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
    print$("    scause: %x, stval: %x\n\n", scause, stval);
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

Stackframe *exception_handler(Stackframe *frame) {
    uint32_t scause = read_csr$(scause);
    uint32_t stval = read_csr$(stval);
    uint32_t user_pc = read_csr$(sepc);

    if (scause & (1 << 31)) {
        log$("Interrupt: %s", interrupts[scause & 0xff]);
    } else {
        panic_handler(frame, scause, stval, user_pc);
    }

    return frame;
}
