#include <allocs/bump.h>
#include <allocs/slab.h>
#include <logger.h>
#include <memory.h>
#include <unistd.h>

static size_t page_amount(size_t size) {
    if (size <= 64) {
        return 64;
    } else if (size <= 128) {
        return 32;
    } else if (size <= 512) {
        return 16;
    } else if (size <= 1024) {
        return 8;
    } else {
        return 4;
    }
}

static void *_alloc(void *ctx, [[maybe_unused]] size_t len) {
    SlabAllocator *alloc = (SlabAllocator *)ctx;
    if (alloc->root == NULL) {
        return NULL;
    }

    void *ptr = (void *)(alloc->root->ptr + sizeof(Slab));
    alloc->root = alloc->root->next;
    return ptr;
}

static void _free(void *ctx, void *ptr, [[maybe_unused]] size_t len) {
    SlabAllocator *alloc = (SlabAllocator *)ctx;
    Slab *free = (Slab *)((uintptr_t)ptr - sizeof(Slab));

    if (free->ptr + sizeof(Slab) != (uintptr_t)ptr) {
        error$("Invalid free %p != %p", free->ptr, ptr);
        return;
    }

    Slab *slab = alloc->root;

    while (1) {
        if (slab->next == NULL)
            break;

        slab = slab->next;
    }

    slab->next = free;
}

SlabAllocator slab_create(uintptr_t size, Allocator *page_alloc) {
    size_t n_page = page_amount(size + sizeof(Slab));
    void *page = page_alloc->alloc(page_alloc, n_page * getpagesize());

    if (page == NULL) {
        return (SlabAllocator){0};
    }

    size_t sz = align_up$(sizeof(Slab) + size, sizeof(void *));
    BumpAllocator bump = bump_allocator_create(page, n_page * getpagesize());
    Slab *root = bump.base.alloc(&bump, sz);
    root->ptr = (uintptr_t)root;

    Slab *slab = root;

    while (1) {
        void *ptr = bump.base.alloc(&bump, sz);

        if (ptr == NULL) {
            slab->next = NULL;
            break;
        }

        slab->next = ptr;
        slab->next->ptr = (uintptr_t)ptr;
        slab = slab->next;
    }

    return (SlabAllocator){
        .base = {
            .alloc = _alloc,
            .free = _free,
            .realloc = 0,
        },
        .root = root,
    };
}
