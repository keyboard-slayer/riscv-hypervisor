#include <stdlib.h>

[[gnu::noreturn]] void abort(void) {
    for (;;)
        __asm__ volatile("wfi");
}
