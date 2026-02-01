#include <string.h>

void *memcpy(void *dst, void const *src, size_t n) {
    char *d = (char *)dst;
    char const *s = (char const *)src;

    while (n--)
        *d++ = *s++;

    return dst;
}

void *memset(void *b, int c, size_t len) {
    char *d = (char *)b;

    while (len--)
        *d++ = (char)c;

    return b;
}
