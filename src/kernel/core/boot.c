#include <logger.h>
#include <stdint.h>
#include <string.h>

#include "interrupt.h"
#include "sbi.h"

extern uint8_t *__bss;
extern uint8_t *__bss_end;

[[gnu::naked, gnu::noreturn, gnu::used, gnu::section(".text.boot")]] void boot(void) {
    __asm__ volatile(
        "la sp, __stack_top\n"
        "j hmain\n"
    );
}

static void npf_impl(int c, [[maybe_unused]] void *ctx) {
    ecall(0, 1, c);
}

[[gnu::noreturn]] void hmain(void) {
    memset(__bss, 0, __bss_end - __bss);
    logger_add_output(npf_impl);
    register_handler();

    log$("Hello, world!");

    for (;;)
        __asm__ volatile("wfi");
}
