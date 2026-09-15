#include "fdt.h"
#include "string.h"

static int path_match(const char *cur, const char *tar) {
    const char *c = cur;
    const char *t = tar;

    while (1) {
        if (*c == '@' && (*t == '/' || *t == '\0')) {
            while (*c != '/' && *c != '\0') {
                c++;
            }
            continue;
        }
        
        if (*c != *t) {
            return 0;
        }

        if (*c == '\0') {
            return 1;
        }
        
        c++;
        t++;
    }
}

// 32bit endian conversion
static inline uint32_t bswap32(uint32_t x) {
    return ((x & 0x000000ffU) << 24) |
           ((x & 0x0000ff00U) << 8)  |
           ((x & 0x00ff0000U) >> 8)  |
           ((x & 0xff000000U) >> 24);
}


// 將目前 ptr 對齊到第一個大於或等於目前位址的 align-byte aligned address。
static inline const void* align_up(const void* ptr, size_t align) {
    return (const void*)(((uintptr_t)ptr + align - 1) & ~(align - 1));
}

int fdt_path_offset(const void* fdt, const char* path) {
    // Parse DTB header
    const struct fdt_header *header = fdt;
    
    // Validate Magic Number
    uint32_t magic = bswap32(header->magic);
    if (magic != 0xd00dfeed) {
        return -1; 
    }

    // Get the addr of structure block
    uint32_t struct_offset = bswap32(header->off_dt_struct);
    const char *struct_base = (const char *)fdt + struct_offset;
    const char *p = struct_base;

    // use char to track current path
    char cur_path[MAX_PATH_LEN];
    cur_path[0] = '\0';
    int idx = 0;
    
    while (1) {
        const char *token_pos = p;
        // Used to calculate node offset when a matching path is found.

        uint32_t token = bswap32(*(const uint32_t *)p);
        // Read the current token with 32bit endian conversion.

        p += sizeof(uint32_t);
        // Skip cur token (4 bytes).
        // P now points to the token payload.

        if (token == FDT_END) {
            break;
        }
        int depth = 0;
        switch (token) {
            case FDT_BEGIN_NODE : {
                const char *node_name = p;
                const char *name = node_name;
                
                if (*node_name) {
                    cur_path[idx++] = '/';
                }

                while (*name) {
                    cur_path[idx++] = *name++;
                }
                cur_path[idx] = '\0';
                
                if (path_match(cur_path, path)) {
                    return (int)(token_pos - struct_base);
                }

                p += (name - node_name) + 1;
                // Used +1 to skip '\0'
                p = align_up(p, 4);

                break;
            }
            
            case FDT_END_NODE: {
                
                while(idx > 0) {
                    idx--;

                    if (cur_path[idx] == '/') {
                        cur_path[idx] = '\0';
                        break;
                    }
                }
                break;
            }
            
            case FDT_PROP: {
                const struct fdt_property *prop = (const struct fdt_property *)p;
                uint32_t len = bswap32(prop->len);

                p += sizeof(struct fdt_property) + len;
                p = align_up(p, 4);
                break;
            }

            case FDT_NOP: {
                break;
            }

            default: {
                return -1;
            }
        }
    }
    return -1;
}

const void* fdt_getprop(const void* fdt,
                        int nodeoffset,
                        const char* name,
                        int* lenp) {
    const struct fdt_header *header = fdt;
    
    uint32_t struct_offset = bswap32(header->off_dt_struct);
    const char *struct_base = (const char*)fdt + struct_offset;
    
    uint32_t strings_offset = bswap32(header->off_dt_strings);
    const char *strings_base = (const char *)fdt + strings_offset;

    const char *node = struct_base + nodeoffset;
    const char *p = node;
    
   
    p += sizeof(uint32_t); // skip FDT_BEGIN_NODE

    const char *node_name = p; // now p points to node name
    int idx = 0;

    while (*node_name) {
        idx++;
        node_name++;
    }
    p += idx + 1;
    p = align_up(p, 4); 
    
    while (1) {
        uint32_t token = bswap32(*(const uint32_t *)p);
        p += sizeof(uint32_t);

        switch (token) {
            case FDT_END_NODE: {
                return NULL;
            }
            
            case FDT_PROP: {
                const struct fdt_property *prop = (const struct fdt_property *)p;
                uint32_t nameoff = bswap32(prop -> nameoff);
                uint32_t len = bswap32(prop->len);
                const char *prop_name = strings_base + nameoff;

                if (!strcmp(prop_name, name)) {
                    *lenp = len;
                    return prop->data;
                }

                p += sizeof(struct fdt_property) + len;
                p = align_up(p, 4);
                break;
            }

            case FDT_NOP: {
                break;
            }

            default: {
                return NULL;
            }
        }
    }
}

uintptr_t fdt_get_uart_base(const void* fdt) {
    int offset = fdt_path_offset(fdt, "/soc/uart");

    if (offset < 0) {
        offset = fdt_path_offset(fdt, "/soc/serial");
    }

    if (offset < 0) return 0; // UART node not found

    int len;
    const void* prop = fdt_getprop(fdt, offset, "reg", &len);

    if (!prop) return 0; // Missing "reg" property
    if (len != sizeof(uint32_t) * 4) return 0; // Unexpected reg format

    const uint32_t *reg = (const uint32_t *)prop;

    return ((uint64_t)bswap32(reg[0]) << 32) | bswap32(reg[1]);

}