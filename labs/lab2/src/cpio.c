#include "cpio.h"
#include "string.h"
#include "uart.h"

#include <stddef.h>
#include <stdint.h>

struct cpio_new_header {
    char magic[6];
    char ino[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char devmajor[8];
    char devminor[8];
    char rdevmajor[8];
    char rdevminor[8];
    char namesize[8];
    char check[8];
};

/**
 * @brief Convert a hexadecimal string to integer
 *
 * @param s hexadecimal string
 * @param n length of the string
 * @param value output parameter for the parsed value
 * @return 0 on success, -1 on invalid input
 */
static int hextoi(const char *s, size_t n, uint32_t *value)  {
    if (s == NULL || value == NULL) return -1;
    if (n == 0 || n > 8) return -1;
    
    uint32_t res = 0;

    for (size_t i = 0; i < n; i++) {
        char c = s[i];

        if (c >= '0' && c <= '9') {
            res = (res << 4) + (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            res = (res << 4) + (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            res = (res << 4) + (c - 'a' + 10);
        } else {
            return -1;
        }
    }

    *value = res;
    return 0;
}


static int mem_cmp(const char *s1, const char *s2, size_t n) {
    const char *a = s1;
    const char *b = s2;

    for(size_t i = 0; i < n; i++) {
        if(a[i] == b[i]) {
            continue;
        } else {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Align a pointer to the first address greater than or equal to the current address
 *        that is aligned to the specified byte boundary.
 *
 * @param ptr Pointer to be aligned.
 * @param align Alignment size in bytes.
 * @return Aligned pointer.
 */
static inline const void *align_up(const void *ptr, size_t align) {
    return (const void *)(((uintptr_t)ptr + align - 1) & ~(align - 1));
}



int initrd_list(const void *start, const void *end) {
    const char *p = (const char *)start; //get start address and iterate the initrd
    const char *end_addr = (const char *) end;

    if (p == NULL || end_addr == NULL || p >= end_addr) return -1;

    while (1) {
        if (end_addr - p < sizeof(struct cpio_new_header)) return -1;

        const struct cpio_new_header *c = (const struct cpio_new_header *) p;

        // Validate Magic Number
        if (mem_cmp(c->magic, "070701", 6) != 0) return -1;

        // move p to the start point of file name
        const char *filename = p + sizeof(struct cpio_new_header);

        uint32_t name_len = 0;
        int res_name_len = hextoi(c->namesize, 8, &name_len);
        
        if (res_name_len < 0) return -1;
        if (name_len > end_addr - filename) return -1;
        if (name_len == 0 || filename[name_len - 1] != '\0') return -1;

        const char *name_end = filename + name_len;

        uint32_t file_len = 0;
        int res_file_len = hextoi(c->filesize, 8, &file_len);

        if (res_file_len < 0) return -1;

        const char *data_start = align_up(name_end, 4);
        if (data_start < name_end) return -1;
        if (data_start > end_addr) return -1;

        if (file_len > end_addr - data_start) return -1;

        if (strcmp(filename, "TRAILER!!!") == 0) break;

        const char *data_end = data_start + file_len;
        const char *next_header = align_up(data_end, 4);
        if (next_header < data_end) return -1;
        if (next_header > end_addr) return -1;
        if (next_header <= p) return -1;

        p = next_header;

        uart_hex((unsigned long)file_len);
        uart_puts(" ");
        uart_puts(filename);
        uart_puts("\n");
    }
    return 0;
}

int initrd_cat(const void *start, const void *end, const char *filename) {
    const char *p = (const char *) start;
    const char *end_addr = (const char *) end;

    if (p == NULL || end_addr == NULL || filename == NULL || p >= end_addr) return -1;

    while(1) {
        if (end_addr - p < sizeof(struct cpio_new_header)) return -1;
        const struct cpio_new_header *c = (const struct cpio_new_header *) p;

        if (mem_cmp(c->magic, "070701", 6) != 0) return -1;

        const char *curr_filename = p + sizeof(struct cpio_new_header);

        uint32_t name_len = 0;
        int ret_name_len = hextoi(c->namesize, 8, &name_len);

        if (ret_name_len < 0) return -1;
        if (name_len > end_addr - curr_filename) return -1;
        if (name_len == 0 || curr_filename[name_len - 1] != '\0') return -1;

        const char *name_end = curr_filename + name_len;

        uint32_t file_len = 0;
        int ret_file_len = hextoi(c->filesize, 8, &file_len);

        if(ret_file_len < 0) return -1;

        const char *data_start = align_up(name_end, 4);
        if (data_start < name_end) return -1;
        if (data_start > end_addr) return -1;
        if (file_len > end_addr - data_start) return -1;

        if (strcmp(curr_filename, "TRAILER!!!") == 0) {
            return -1;
        } else if (strcmp(curr_filename, filename) == 0) {
            uart_puts(filename);
            uart_puts("\n");
            
            for (uint32_t i = 0; i < file_len; i++) {
                uart_putc(data_start[i]);
            }
            return 0;
        } 

        const char *data_end = data_start + file_len;
        const char *next_header = align_up(data_end, 4);
        
        if (next_header < data_end) return -1;
        if (next_header > end_addr) return -1;
        if (next_header <= p) return -1;

        p = next_header;
    }
}