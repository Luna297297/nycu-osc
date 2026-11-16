#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include "memory_layout.h"

#define KERNEL_LOAD_ADDR BOOTLOADER_RELOCATION_ADDR

void bootloader_load(uint64_t hartid,
                     const void *fdt,
                     uintptr_t destination,
                     uintptr_t limit);

#endif
