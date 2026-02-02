
#pragma once

#include <stdint.h>
#include <traits/allocator.h>

#define PAGE_SIZE 4096

typedef struct _slab {
    uintptr_t ptr;
    struct _slab *next;
} Slab;

typedef struct {
    Allocator base;
    Slab *root;
} SlabAllocator;

SlabAllocator slab_create(uintptr_t size, Allocator *page_alloc);
