#include <logger.h>
#include <stddef.h>
#include <string.h>

#include "hypervisor.h"
#include "paging.h"
#include "pmm.h"
#include "regs.h"

#define KERNEL_ENTRY 0x100000

uint8_t const kernel[] = {
    0x6f, 0x00, 0x00, 0x00
};

void enter_hs_mode(void) {
    PhysObj kernel_page = pmm_allocz(1);
    if (kernel_page.len == 0)
        panic$("Failed to allocate memory for kernel page");

    PhysObj kernel_image = pmm_allocz(1);
    if (kernel_image.len == 0)
        panic$("Failed to allocate memory for kernel image");

    memcpy((void *)kernel_image.base, kernel, sizeof(kernel));

    if (paging_map(
            (uintptr_t *)kernel_page.base, KERNEL_ENTRY, kernel_image.base,
            MAPPING_READ | MAPPING_WRITE | MAPPING_EXECUTE | MAPPING_USER
        ) != 0)
        panic$("Failed to map kernel page");

    Hstatus hstatus = {0};
    hstatus.vsxl = 2;
    hstatus.spv = 1;
    write_csr$(hstatus, hstatus.bits);

    uint64_t sstatus = read_csr$(sstatus);
    write_csr$(sstatus, sstatus | (1UL << 8));

    write_csr$(hgatp, hgatp$(kernel_page.base));
    write_csr$(sepc, KERNEL_ENTRY);
    __asm__ volatile("sret");
}
