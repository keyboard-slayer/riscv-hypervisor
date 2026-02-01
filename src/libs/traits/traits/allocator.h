#pragma once

#include <stddef.h>

typedef struct {
    void *(*alloc)(void *ctx, size_t len);
    void *(*realloc)(void *ctx, void *ptr, size_t len);
    void (*free)(void *ctx, void *ptr, size_t len);
} Allocator;
