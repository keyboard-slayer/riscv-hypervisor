#pragma once

#include <macros.h>
#include <stddef.h>

typedef struct {
    size_t err;
    size_t value;
} SbiReturn;

SbiReturn __ecall_impl(size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t fid, size_t eid);

#define ecall(fid, eid, ...) __ecall(fid, eid, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)
#define __ecall(fid, eid, n, ...) __ecall_(n, fid, eid, __VA_ARGS__)
#define __ecall_(n, fid, eid, ...) __ecall##n(fid, eid, __VA_ARGS__)

#define __ecall1(fid, eid, arg0) __ecall_impl(arg0, 0, 0, 0, 0, 0, fid, eid)
