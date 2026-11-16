#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include "memory_layout.h"

#ifdef CONFIG_SELF_RELOCATE
#define KERNEL_LOAD_ADDR STANDARD_KERNEL_LOAD_ADDR
#else
#define KERNEL_LOAD_ADDR BOOTLOADER_RELOCATION_ADDR
#endif

void bootloader_load(uint64_t hartid,
                     const void *fdt,
                     uintptr_t destination,
                     uintptr_t limit);

#endif
