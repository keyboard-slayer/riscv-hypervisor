#pragma once

#include <stdint.h>

enum mapping_flags {
    MAPPING_VALID = 1 << 0,
    MAPPING_READ = 1 << 1,
    MAPPING_WRITE = 1 << 2,
    MAPPING_EXECUTE = 1 << 3,
    MAPPING_USER = 1 << 4,
};

#define vpn$(addr, lvl) ((addr >> (12 + 9 * lvl)) & 0x1FF)
#define paddr$(pte) ((((uint64_t)(pte)) >> 10) << 12)
#define hgatp$(x) (9ULL << 60 | ((uintptr_t)(x) >> 12))

int paging_map(uintptr_t *table, uintptr_t vaddr, uintptr_t paddr, uint32_t flag);
