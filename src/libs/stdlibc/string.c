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
        *d++ = c;
    return b;
}

size_t strlen(char const *s) {
    size_t i;
    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}

int strncmp(char const *s1, char const *s2, size_t n) {
    while (*s1 == *s2 && *s1 && n) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0)
        return 0;

    return (int)*s1 - (int)*s2;
}
