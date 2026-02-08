#include <logger.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include "dtb.h"
#include "pmm.h"

static PmmBitmap bitmap = {0};
static size_t offset = 0;

static bool bitmap_is_bit_set(size_t bit) {
    return bitmap.bitmap[bit / 8] & (1 << (bit % 8));
}

static void pmm_mark_free(uintptr_t base, size_t len) {
    size_t start = align_up$(base, getpagesize()) / getpagesize();
    size_t end = align_down$(base + len, getpagesize()) / getpagesize();

    for (size_t i = start; i < end; i++) {
        bitmap.bitmap[i / 8] &= ~(1 << (i % 8));
    }
}

static void pmm_mark_used(uintptr_t base, size_t len) {
    size_t start = align_up$(base, getpagesize()) / getpagesize();
    size_t end = align_down$(base + len, getpagesize()) / getpagesize();

    for (size_t i = start; i < end; i++) {
        bitmap.bitmap[i / 8] |= 1 << (i % 8);
    }
}

void pmm_init(DTBNode *dtb, Allocator *alloc) {
    DTBNode *mem = dtb_lookup(dtb, "memory");
    if (mem == NULL)
        panic$("Failed to find memory node in device tree");

    RegValue mem_reg = dtb_lookup_reg(mem);
    if (mem_reg.len == 0) {
        warn$("Memory node size is larger than 4GB. Defaulting to 4GB");
        mem_reg.len = align_down$(gib$(4) - 1, 4096);
    }

    debug$("Memory base: %p, size: %d bytes", mem_reg.addr, mem_reg.len);
    offset = mem_reg.addr;

    bitmap.len = align_up$(mem_reg.len / (getpagesize() * 8), getpagesize());
    bitmap.last = 0;
    bitmap.bitmap = (uint8_t *)alloc->alloc(alloc, bitmap.len);

    if (bitmap.bitmap == NULL)
        panic$("Failed to allocate memory for physical memory bitmap");

    memset(bitmap.bitmap, 0x00, bitmap.len);

    DTBNode *reserved = dtb_lookup(dtb, "reserved-memory");
    if (reserved == NULL)
        panic$("Failed to find reserved-memory node in device tree");

    DTBNode *child = reserved->children;
    while (child != NULL) {
        debug$("Marking reserved memory region: start: %p, size: %d bytes", dtb_lookup_reg(child).addr, dtb_lookup_reg(child).len);
        RegValue reg = dtb_lookup_reg(child);
        pmm_mark_used(reg.addr - offset, reg.len);
        child = child->next;
    }
}

PhysObj pmm_allocz(size_t pages) {
    PhysObj obj = pmm_alloc(pages);
    if (obj.len == 0)
        return obj;

    memset((void *)obj.base, 0, obj.len);
    return obj;
}

PhysObj pmm_alloc(size_t pages) {
    static bool try_again = false;
    size_t size = 0;
    size_t start_index = 0;

    for (size_t i = bitmap.last; i < bitmap.len * 8; i++) {
        if (!bitmap_is_bit_set(i)) {
            if (size == 0)
                start_index = i;

            if (size++ == pages) {
                pmm_mark_used(start_index * getpagesize(), pages * getpagesize());
                bitmap.last = i + 1;
                try_again = false;
                return (PhysObj){.base = start_index * getpagesize() + offset, .len = pages * getpagesize()};
            }
        } else
            size = 0;
    }

    if (!try_again) {
        warn$("End of the bitmap reached, trying again");
        try_again = true;
        bitmap.last = 0;
        return pmm_alloc(pages);
    } else
        panic$("Out of physical memory");
    __builtin_unreachable();
}

void pmm_free(PhysObj obj) {
    pmm_mark_free(obj.base - offset, obj.len);
}

// === ALLOCATOR ===

static void *_alloc(__attribute__((unused)) void *ctx, size_t len) {
    PhysObj obj = pmm_alloc(len);
    if (obj.len == 0)
        return NULL;

    return (void *)obj.base;
}

static void _free(__attribute__((unused)) void *ctx, void *ptr, size_t len) {
    PhysObj obj = {.base = (uintptr_t)ptr, .len = len};
    pmm_free(obj);
}

Allocator pmm_allocator(void) {
    return (Allocator){
        .alloc = _alloc,
        .free = _free,
        .realloc = NULL,
    };
}
