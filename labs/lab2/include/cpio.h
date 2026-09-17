#ifndef CPIO_H
#define CPIO_H

int initrd_list(const void *start, const void *end);
int initrd_cat(const void *start, const void *end, const char *filename);

#endif