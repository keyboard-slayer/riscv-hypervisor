#pragma once

#define align_up$(x, align) (((x) + (align) - 1) & ~((align) - 1))

#define align_down$(x, align) ((x) & ~((align) - 1))

#define aligned$(x, align) (x % align == 0)

#define kib$(x) ((uintptr_t)(x) * 1024)

#define mib$(x) (kib$(x) * 1024)

#define gib$(x) (mib$(x) * 1024)
