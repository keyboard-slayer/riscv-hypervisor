#include <allocs/bump.h>
#include <dtb.h>
#include <logger.h>
#include <stdint.h>
#include <string.h>

#include "exception.h"
#include "hypervisor.h"
#include "pmm.h"
#include "sbi.h"

extern uint8_t __bss[];
extern uint8_t __bss_end[];
extern uint8_t __early_heap[];
extern uint8_t __early_heap_end[];

[[gnu::naked, gnu::noreturn, gnu::used, gnu::section(".text.boot")]] void boot(void) {
    __asm__ volatile(
        "la sp, __stack_top\n"
        "j hmain\n"
    );
}

static void npf_impl(int c, [[maybe_unused]] void *ctx) {
    ecall(0, 1, c);
}

[[gnu::noreturn]] void hmain([[maybe_unused]] size_t hart_id, uintptr_t dtb) {
    memset(__bss, 0, __bss_end - __bss);
    logger_add_output(npf_impl);
    register_handler();

    log$("Hello World!");

    BumpAllocator alloc = bump_allocator_create(__early_heap, __early_heap_end - __early_heap);
    DTBNode *root = dtb_init(dtb, (Allocator *)&alloc);

    if (root == NULL)
        panic$("Failed to parse device tree blob");

    pmm_init(root, (Allocator *)&alloc);
    enter_hs_mode();
    log$("Hanging...");

    for (;;)
        __asm__ volatile("wfi");
}
