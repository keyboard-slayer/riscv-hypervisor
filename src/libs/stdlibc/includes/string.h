#pragma once

#include <stddef.h>

void *memcpy(void *dst, void const *src, size_t n);

void *memset(void *b, int c, size_t len);

size_t strlen(char const *s);

int strncmp(char const *s1, char const *s2, size_t n);
