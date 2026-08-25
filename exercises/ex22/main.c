#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct cpio_t {
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
 * @return integer value
 */
static int hextoi(const char* s, int n) {
    int r = 0;
    while (n-- > 0) {
        r = r << 4;
        // 每個十六進位字元只佔 4 bits，因此每讀入一個新的 hex digit時，
        // 要先將目前結果左移 4 bits，才能正確加入該數值。
        if (*s >= 'A')
            r += *s++ - 'A' + 10;
        else if (*s >= 0)
            r += *s++ - '0';
    }
    return r;
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

/**
 * @brief Align a number to the nearest multiple of a given number
 *
 * @param n number
 * @param byte alignment
 * @return aligned number
 *
 * static int align(int n, int byte) {
 *    return (n + byte - 1) & ~(byte - 1);
 * }
**/


void initrd_list(const void* rd) {
    const char *p = (const char *)rd; //get start address and iterate the initrd

    while (1) {
        const struct cpio_t *c = (const struct cpio_t *) p;

        // Validate Magic Number
        if (memcmp(c->magic, "070701", 6) != 0) return;

        // move p to the start point of file name
        const char *filename = p + sizeof(struct cpio_t);

        // Print Info of current file
        int file_len = hextoi(c->filesize, 8);
        int name_len = hextoi(c->namesize, 8);


        if (strcmp(filename, "TRAILER!!!") == 0) break;

        printf("%d %s\n", file_len, filename);

        p = filename;
        p += name_len;
        p = align_up(p, 4); // first padding
        p += file_len;
        p = align_up(p, 4); // second padding
    }
}

void initrd_cat(const void* rd, const char* filename) {
    const char *p = (const char *) rd;

    while(1) {
        const struct cpio_t *c = (const struct cpio_t *) p;

        if (memcmp(c->magic, "070701", 6) != 0) return;

        const char *cur_filename = p + sizeof(struct cpio_t);

        int name_len = hextoi(c->namesize, 8);
        int file_len = hextoi(c->filesize, 8);

        if (strcmp(cur_filename, "TRAILER!!!") == 0) {
            printf("%s : No such file.\n", filename);
            return;
        } else if (strcmp(cur_filename, filename) == 0) {
            printf("%s : %d\n", filename, file_len);
            return;
        } else {
            p = cur_filename;
            p += name_len;
            p = align_up(p, 4);
            p += file_len;
            p = align_up(p, 4);
        }
    }
}

int main() {
    /* Prepare the initial RAM disk */
    FILE* fp = fopen("initramfs.cpio", "rb"); // 以二進位讀檔
    if (!fp) {
        perror("fopen"); // 如果無法開檔，以 fopen: sys message 印出錯誤訊息
        return EXIT_FAILURE;
    }
    fseek(fp, 0, SEEK_END); // 將指標移動到檔案結尾
    long sz = ftell(fp); // 計算指標距離檔案開頭的位元組數 aka 取得檔案大小
    void* rd = malloc(sz); // 分配記憶體
    fseek(fp, 0, SEEK_SET); // 將指標移動到檔案開頭
    if (fread(rd, 1, sz, fp) != sz) { // 如果存入 buffer 的大小不等於檔案大小
        fprintf(stderr, "Failed to read the initial RAM disk\n");
        free(rd);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    initrd_list(rd); //列出 buffer 中所有來自 initramfs.cpio 的檔案名稱和大小
    initrd_cat(rd, "osc.txt"); // 尋找並輸出 osc.txt 的內容；若不存在則回報
    initrd_cat(rd, "test.txt"); 

    free(rd); // free buffer，釋放記憶體
    return 0;
}
