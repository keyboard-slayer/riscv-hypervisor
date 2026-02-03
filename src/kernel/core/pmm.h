#pragma once

#include <dtb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <traits/allocator.h>

typedef struct
{
    size_t len;
    size_t last;
    uint8_t *bitmap;
} PmmBitmap;

typedef struct
{
    size_t len;
    uintptr_t base;
} PhysObj;

struct pmm_alloc_param {
    size_t pages;
    bool low;
};

PhysObj pmm_alloc(size_t pages);

void pmm_init(DTBNode *dtb, Allocator *alloc);

void pmm_free(PhysObj obj);

Allocator pmm_allocator(void);
