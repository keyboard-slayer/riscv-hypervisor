#pragma once

#include "regs.h"

extern void interrupt_kernel(void);

void register_handler(void);

Stackframe *exception_handler(Stackframe *frame);

void trap_handler(void);
