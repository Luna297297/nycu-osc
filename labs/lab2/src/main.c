#include "bootloader.h"
#include "fdt.h"
#include "sbi.h"
#include "shell.h"
#include "uart.h"

#include <stdint.h>

static void print_boot_info(uint64_t hartid,
                            const void *fdt,
                            uintptr_t uart_base,
                            uintptr_t initrd_start,
                            uintptr_t initrd_end) {
    uart_puts("\nStarting kernel ...\n");

    uart_puts("Hart ID: ");
    uart_hex(hartid);
    uart_puts("\n");

    uart_puts("FDT Addr: ");
    uart_hex((unsigned long)fdt);
    uart_puts("\n");

    uart_puts("UART Base Addr: ");
    uart_hex((unsigned long)uart_base);
    uart_puts("\n");

    uart_puts("Initrd Start Addr: ");
    uart_hex((unsigned long)initrd_start);
    uart_puts("\n");

    uart_puts("Initrd End Addr: ");
    uart_hex((unsigned long)initrd_end);
    uart_puts("\n");

    uart_puts("Initrd Size: ");
    uart_hex((unsigned long)(initrd_end - initrd_start));
    uart_puts("\n");

    uart_puts("SBI specification version: ");
    uart_hex(sbi_get_spec_version());
    uart_puts("\n");
}

void start_kernel(uint64_t hartid, const void *fdt) {
    uintptr_t uart_base = fdt_get_uart_base(fdt);
    if (uart_base == 0) {
        while (1) {
        }
    }
    uart_init(uart_base);

    uintptr_t initrd_start_addr;
    uintptr_t initrd_end_addr;
    if (fdt_get_initrd_range(fdt,
                             &initrd_start_addr,
                             &initrd_end_addr) < 0) {
        uart_puts("Failed to get Initrd address.\n");
        while (1) {
        }
    }

    const void *initrd_start = (const void *)initrd_start_addr;
    const void *initrd_end = (const void *)initrd_end_addr;

    print_boot_info(hartid,
                    fdt,
                    uart_base,
                    initrd_start_addr,
                    initrd_end_addr);

    while (1) {
        char buffer[64];

        uart_puts("opi-rv2> ");

        to_buffer(buffer, sizeof(buffer));

        enum shell_action Status = command_parser(buffer, initrd_start, initrd_end);

        if (Status == SHELL_ACTION_LOAD) {
            bootloader_load(hartid, fdt, 0x82000000UL, initrd_start_addr);
        }
    }
}

