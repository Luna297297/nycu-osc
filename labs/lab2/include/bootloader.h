#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>

void bootloader_load(uint64_t hartid,
                     const void *fdt,
                     uintptr_t destination,
                     uintptr_t limit);

#endif
