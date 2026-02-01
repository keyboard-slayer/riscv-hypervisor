#include <endian.h>

uint32_t from_be32(uint32_t x) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#else
    return x;
#endif // __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
}
