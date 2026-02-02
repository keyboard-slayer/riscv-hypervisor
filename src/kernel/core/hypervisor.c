#include <logger.h>
#include <stddef.h>

#include "hypervisor.h"
#include "regs.h"

void enter_hs_mode(void) {
    Hstatus hstatus = {.bits = read_csr$(hstatus)};
    hstatus.spv = 1;
    write_csr$(hstatus, hstatus.bits);

    uint64_t sstatus = read_csr$(sstatus);
    write_csr$(sstatus, sstatus | (1UL << 8));

    write_csr$(sepc, 0x1234abcd);
    __asm__ volatile("sret");
}
