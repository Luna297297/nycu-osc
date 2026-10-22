#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>

#if defined(CONFIG_BOARD_QEMU)
#define KERNEL_LOAD_ADDR 0x82000000UL
#elif defined(CONFIG_BOARD_ORANGEPI)
#define KERNEL_LOAD_ADDR 0x20000000UL
#else
#error "Unsupported board"
#endif

void bootloader_load(uint64_t hartid,
                     const void *fdt,
                     uintptr_t destination,
                     uintptr_t limit);

#endif
