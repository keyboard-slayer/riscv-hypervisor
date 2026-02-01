#include <endian.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "dtb.h"

static uint32_t read_offset(uint8_t **offset) {
    uint32_t value = from_be32(*(uint32_t *)*offset);
    *offset += sizeof(uint32_t);
    return value;
}

static DTBNode *dtb_parse(FDTHeader *dtb_header, Allocator *alloc) {
    if (dtb_header == NULL) {
        printf("Device tree blob not initialized\n");
        return NULL;
    }

    DTBNode *current_node = NULL;
    DTBNode *root = NULL;
    char const *strtab = (char const *)((uint8_t *)dtb_header + from_be32(dtb_header->off_dt_strings));
    uint8_t *offset = (uint8_t *)dtb_header + from_be32(dtb_header->off_dt_struct);
    int found_end = 0;

    while (!found_end) {
        uint32_t token = read_offset(&offset);

        switch (token) {
        case FDT_BEGIN_NODE: {
            char const *node_name = (char const *)offset;
            offset += strlen(node_name) + 1;
            offset = (uint8_t *)align_up$((uintptr_t)offset, 4);

            if (!root) {
                if (*node_name) {
                    printf("Invalid device tree blob. Expected root node to be empty\n");
                    return NULL;
                }

                root = alloc->alloc(alloc, sizeof(DTBNode));
                if (root == NULL) {
                    printf("Failed to allocate memory for device tree node\n");
                    return NULL;
                }

                memset(root, 0, sizeof(DTBNode));
                root->name[0] = '/';
                root->name[1] = '\0';
                current_node = root;
                break;
            }

            DTBNode *new_node = alloc->alloc(alloc, sizeof(DTBNode));
            if (new_node == NULL) {
                printf("Failed to allocate memory for device tree node\n");
                return NULL;
            }

            memset(new_node, 0, sizeof(DTBNode));

            if (*node_name)
                memcpy(new_node->name, node_name, strlen(node_name));
            else
                memcpy(new_node->name, "<anonymous>", sizeof("<anonymous>"));

            new_node->parent = current_node;

            if (current_node->children == NULL)
                current_node->children = new_node;
            else {
                DTBNode *last_child = current_node->children;
                while (last_child->next != NULL)
                    last_child = last_child->next;
                last_child->next = new_node;
            }

            current_node = new_node;
            break;
        }

        case FDT_END_NODE: {
            current_node = current_node->parent;
            break;
        }

        case FDT_PROP: {
            uint32_t len = read_offset(&offset);
            uint32_t nameoff = read_offset(&offset);

            DTBProp *prop = alloc->alloc(alloc, sizeof(DTBProp));
            if (prop == NULL) {
                printf("Failed to allocate memory for device tree property\n");
                return NULL;
            }

            char const *prop_name = strtab + nameoff;
            prop->len = len;
            prop->value = offset;
            prop->next = NULL;
            memcpy(prop->name, prop_name, strlen(prop_name));

            offset += len;
            offset = (uint8_t *)align_up$((uintptr_t)offset, 4);

            if (current_node->props == NULL)
                current_node->props = prop;
            else {
                DTBProp *last_prop = current_node->props;
                while (last_prop->next != NULL)
                    last_prop = last_prop->next;
                last_prop->next = prop;
            }

            break;
        }

        case FDT_END:
            found_end = 1;
            break;

        case FDT_NOP:
            break;

        default: {
            printf("Unknown token %x\n", token);
            return NULL;
        }
        }
    }

    return root;
}

DTBNode *dtb_init(uintptr_t dtb, Allocator *alloc) {
    FDTHeader *dtb_header = (FDTHeader *)dtb;

    if (from_be32(dtb_header->magic) != DTB_MAGIC) {
        printf("Invalid device tree blob magic number. Excecting %x, got %x\n", DTB_MAGIC, from_be32(dtb_header->magic));
        return NULL;
    }

    return dtb_parse(dtb_header, alloc);
}

DTBNode *dtb_lookup(DTBNode *root, char const *name) {
    DTBNode *node = root->children;

    while (node->next != NULL) {
        if (strncmp(name, node->name, strlen(name)) == 0)
            return node;

        node = node->next;
    }

    return NULL;
}

DTBProp *dtb_lookup_prop(DTBNode *node, char const *name) {
    DTBProp *prop = node->props;

    while (prop != NULL) {
        if (strncmp(name, prop->name, strlen(name)) == 0)
            return prop;

        prop = prop->next;
    }

    return NULL;
}

RegValue dtb_lookup_reg(DTBNode *node) {
    RegValue reg = {0};

    DTBProp *prop = dtb_lookup_prop(node, "reg");
    if (prop == NULL) {
        printf("Failed to find reg property in device tree node %s\n", node->name);
        return reg;
    }

    uint8_t *offset = (uint8_t *)prop->value;

    DTBProp *address_cells_prop = dtb_lookup_prop(node->parent, "#address-cells");
    if (address_cells_prop == NULL) {
        printf("Failed to find #address-cells property in device tree node %s\n", node->parent->name);
        return reg;
    }

    DTBProp *size_cells_prop = dtb_lookup_prop(node->parent, "#size-cells");
    if (size_cells_prop == NULL) {
        printf("Failed to find #size-cells property in device tree node %s\n", node->parent->name);
        return reg;
    }

    size_t address_cells = from_be32(*(uint32_t *)address_cells_prop->value);
    size_t size_cells = from_be32(*(uint32_t *)size_cells_prop->value);

    uint64_t tmp_addr = 0;
    uint64_t tmp_len = 0;

    for (size_t i = 0; i < address_cells; i++)
        tmp_addr = (tmp_addr << 32) | read_offset(&offset);

    for (size_t i = 0; i < size_cells; i++)
        tmp_len = (tmp_len << 32) | read_offset(&offset);

    reg.addr = tmp_addr;
    reg.len = tmp_len;

    return reg;
}
