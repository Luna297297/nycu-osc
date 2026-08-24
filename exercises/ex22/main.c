#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * @brief Align a number to the nearest multiple of a given number
 *
 * @param n number
 * @param byte alignment
 * @return aligned number
 */
static int align(int n, int byte) {
    return (n + byte - 1) & ~(byte - 1);
}

void initrd_list(const void* rd) {
    // TODO: Implement this function
}

void initrd_cat(const void* rd, const char* filename) {
    // TODO: Implement this function
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
