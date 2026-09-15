#include <stdint.h>

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009

#define MAX_PATH_LEN   256

/* FDT Structures */
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

struct fdt_property {
    uint32_t len;
    uint32_t nameoff;
    char data[];
};


/* Internal Helpers */
// 32bit endian conversion
static inline uint32_t bswap32(uint32_t x);
// 64bit endian conversion
static inline uint64_t bswap64(uint64_t x);
// 將目前 ptr 對齊到第一個大於或等於目前位址的 align-byte aligned address。
static inline const void* align_up(const void* ptr, size_t align);

/* Public APIs */
int fdt_path_offset(const void* fdt, const char* path);

const void* fdt_getprop(const void* fdt,
                        int nodeoffset,
                        const char* name,
                        int* lenp);

uintptr_t fdt_get_uart_base(const void* fdt);