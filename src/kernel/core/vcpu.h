#pragma once

#include <memory.h>
#include <stdint.h>

#include "regs.h"

#define STACK_SIZE kib$(512)

typedef struct {
    Stackframe frame;
    uint64_t hstatus;
    uint64_t hgatp;
    uint64_t sstatus;
    uint64_t sepc;
    uint64_t host_sp;
} Vcpu;

int cpu_init(Vcpu *self, uintptr_t *page, uintptr_t sepc);

void cpu_run(Vcpu *self);
