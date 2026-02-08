#include <errno.h>
#include <logger.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include "paging.h"
#include "pmm.h"

int paging_map(uintptr_t *table, uintptr_t vaddr, uintptr_t paddr, uint32_t flag) {
    if (!aligned$(vaddr, getpagesize()) || !aligned$(paddr, getpagesize()))
        return -EINVAL;

    for (size_t level = 3; level > 0; level--) {
        size_t vpn = vpn$(vaddr, level);
        if ((table[vpn] & MAPPING_VALID) == 0) {
            PhysObj page_phys = pmm_allocz(1);
            if (page_phys.base == 0)
                return -ENOMEM;

            table[vpn] = (page_phys.base >> 12) << 10 | MAPPING_VALID;
        }

        table = (uintptr_t *)paddr$(table[vpn]);
    }

    size_t vpn0 = vpn$(vaddr, 0);
    if ((table[vpn0] & MAPPING_VALID) == MAPPING_VALID)
        return -EEXIST;

    table[vpn0] = ((paddr >> 12) << 10) | flag | MAPPING_VALID;
    return 0;
}
