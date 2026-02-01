#pragma once

#include <stdint.h>
#include <traits/allocator.h>

// TODO: Add locking mechanism to allocation

typedef struct {
    Allocator base;
    void* heap_start;
    void* heap_end;
    void* next;
    size_t allocations;
} BumpAllocator;

BumpAllocator bump_allocator_create(void* start, size_t len);
