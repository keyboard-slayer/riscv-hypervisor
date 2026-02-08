#include <logger.h>
#include <stddef.h>
#include <string.h>

#include "hypervisor.h"
#include "paging.h"
#include "pmm.h"
#include "vcpu.h"

#define KERNEL_ENTRY 0x100000

uint8_t const kernel[] = {
    0x85, 0x48, 0x01, 0x48, 0x13, 0x05, 0x10, 0x04, 0x73, 0x00, 0x00, 0x00,
    0x13, 0x05, 0x20, 0x04, 0x73, 0x00, 0x00, 0x00, 0x13, 0x05, 0x30, 0x04,
    0x73, 0x00, 0x00, 0x00, 0x01, 0xa0
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

    Vcpu cpu = {0};
    if (cpu_init(&cpu, (uintptr_t *)kernel_page.base, KERNEL_ENTRY) != 0)

        panic$("Failed to initialize CPU");

    cpu_run(&cpu);
}
