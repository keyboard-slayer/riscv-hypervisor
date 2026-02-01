#pragma once

#include <stdint.h>
#include <traits/allocator.h>

#define DTB_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE 0x1
#define FDT_END_NODE 0x2
#define FDT_PROP 0x3
#define FDT_NOP 0x4
#define FDT_END 0x9

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
} FDTHeader;

typedef struct _DTBProp {
    char name[64];
    uint32_t len;
    void *value;
    struct _DTBProp *next;
} DTBProp;

typedef struct _DTBNode {
    char name[64];

    DTBProp *props;
    struct _DTBNode *children;
    struct _DTBNode *next;
    struct _DTBNode *parent;
} DTBNode;

typedef struct {
    uintptr_t addr;
    size_t len;
} RegValue;

DTBNode *dtb_init(uintptr_t dtb, Allocator *alloc);

DTBNode *dtb_lookup(DTBNode *root, char const *name);

DTBProp *dtb_lookup_prop(DTBNode *node, char const *name);

RegValue dtb_lookup_reg(DTBNode *node);
