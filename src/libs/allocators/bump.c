#include <allocs/bump.h>
#include <logger.h>
#include <memory.h>
#include <stdint.h>

static void *alloc(void *ctx, size_t len) {
    size_t aligned_len = len & 7 ? align_up$(len, 8) : len;

    BumpAllocator *self = (BumpAllocator *)ctx;
    uint8_t *ptr = self->next;
    self->next = (void *)((uint8_t *)self->next + aligned_len);

    if ((uintptr_t)self->next > (uintptr_t)self->heap_end) {
        return NULL;
    }

    self->allocations++;
    return (void *)ptr;
}

static void free(void *ctx, __attribute__((unused)) void *ptr, __attribute__((unused)) size_t len) {
    BumpAllocator *self = (BumpAllocator *)ctx;
    self->allocations--;

    if (self->allocations == 0) {
        self->next = self->heap_start;
    }
}

BumpAllocator bump_allocator_create(void *start, size_t len) {
    return (BumpAllocator){
        .base = {
            .alloc = alloc,
            .free = free,
            .realloc = NULL,
        },
        .heap_start = start,
        .heap_end = (char *)start + len,
        .next = start,
        .allocations = 0,
    };
}
