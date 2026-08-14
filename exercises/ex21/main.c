#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009
#define MAX_PATH_LEN   256

struct fdt_header {
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
};

// 32bit endian conversion
static inline uint32_t bswap32(uint32_t x) {
    return __builtin_bswap32(x);
}

// 64bit endian conversion
static inline uint64_t bswap64(uint64_t x) {
    return __builtin_bswap64(x);
}

// 將目前 ptr 對齊到第一個大於或等於目前位址的 align-byte aligned address。
static inline const void* align_up(const void* ptr, size_t align) {
    return (const void*)(((uintptr_t)ptr + align - 1) & ~(align - 1));
}

int fdt_path_offset(const void* fdt, const char* path) {
    // Get DTB base addr
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
    char cur_path[MAX_PATH_LEN] = "";
    
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
                
                if (strlen(node_name) > 0) {
                    strcat(cur_path, "/");
                    strcat(cur_path, node_name);
                }

                if (strcmp(cur_path, path) == 0) {
                    return (int)(token_pos - struct_base);
                }

                p += strlen(node_name) + 1;
                // Used +1 to skip '\0'
                p = align_up(p, 4);

                break;
            }
            
            case FDT_END_NODE: {
                char *last = strrchr(cur_path, '/');

                if (last != NULL) {
                    *last = '\0';
                }

                break;
            }
            
            case FDT_PROP: {
                const
            }

            case FDT_NOP: {
                continue;
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
    // TODO: Implement this function
}

int main() {
    /* Prepare the device tree blob */
    FILE* fp = fopen("qemu.dtb", "rb");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    void* fdt = malloc(sz);
    fseek(fp, 0, SEEK_SET);
    if (fread(fdt, 1, sz, fp) != sz) {
        fprintf(stderr, "Failed to read the device tree blob\n");
        free(fdt);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    /* Find the node offset */
    int offset = fdt_path_offset(fdt, "/cpus/cpu@0/interrupt-controller");
    if (offset < 0) {
        fprintf(stderr, "fdt_path_offset\n");
        free(fdt);
        return EXIT_FAILURE;
    }

    /* Get the node property */
    int len;
    const void* prop = fdt_getprop(fdt, offset, "compatible", &len);
    if (!prop) {
        fprintf(stderr, "fdt_getprop\n");
        free(fdt);
        return EXIT_FAILURE;
    }
    printf("compatible: %.*s\n", len, (const char*)prop);

    offset = fdt_path_offset(fdt, "/memory");
    prop = fdt_getprop(fdt, offset, "reg", &len);
    const uint64_t* reg = (const uint64_t*)prop;
    printf("memory: base=0x%lx size=0x%lx\n", bswap64(reg[0]), bswap64(reg[1]));

    offset = fdt_path_offset(fdt, "/chosen");
    prop = fdt_getprop(fdt, offset, "linux,initrd-start", &len);
    const uint64_t* initrd_start = (const uint64_t*)prop;
    printf("initrd-start: 0x%lx\n", bswap64(initrd_start[0]));

    free(fdt);
    return 0;
}
