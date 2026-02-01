#include <logger.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupt.h"
#include "regs.h"

static char const *low_exceptions[] = {
    "instruction address misaligned",
    "instruction access fault",
    "illegal instruction",
    "breakpoint",
    "load address misaligned",
    "load access fault",
    "store/AMO address misaligned",
    "store/AMO access fault",
    "environment call from U/VU-mode",
    "environment call from HS-mode",
    "environment call from VS-mode",
    "environment call from M-mode",
    "instruction page fault",
    "load page fault",
    "store/AMO page fault",
    "instruction guest-page fault",
    "load guest-page fault",
    "virtual instruction",
    "store/AMO guest-page fault",
};

static char const *high_exceptions[] = {
    "user software interrupt",
    "supervisor software interrupt",
    "hypervisor software interrupt",
    "machine software interrupt",
    "user timer interrupt",
    "supervisor timer interrupt",
    "hypervisor timer interrupt",
    "machine timer interrupt",
    "user external interrupt",
    "supervisor external interrupt",
    "hypervisor external interrupt",
    "machine external interrupt",
};

[[gnu::used, gnu::section(".text.stvec")]]
void handler(void) {
    size_t cause = read_csr(scause);
    size_t sepc = read_csr(sepc);
    size_t stval = read_csr(stval);

    char const *message = NULL;

    if (cause & HIGH_BIT)
        message = high_exceptions[cause & 0xFFF];
    else
        message = low_exceptions[cause];

    panic$("Exception occurred!\n  cause: %s (%lu)\n  sepc: 0x%lx\n  stval: 0x%lx", message, cause, sepc, stval);
}

void register_handler(void) {
    write_csr$(stvec, (size_t)handler);
}
