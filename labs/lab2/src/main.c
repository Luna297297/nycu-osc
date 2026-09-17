#include "cpio.h"
#include "fdt.h"
#include "sbi.h"
#include "shell.h"
#include "string.h"
#include "uart.h"


#include <stdint.h>

/*
void start_kernel() {
    while (1) {
        uart_puts("LUNA\n");
    }
}
*/

void start_kernel(uint64_t hartid, const void *fdt) {
    uintptr_t detected_uart_base = fdt_get_uart_base(fdt);

    if (detected_uart_base == 0) {
        while(1) { };
    } else {
        uart_init(detected_uart_base);
    }

    uintptr_t initrd_start_addr;
    uintptr_t initrd_end_addr;
    int ret_get_initrd_range = fdt_get_initrd_range(fdt, &initrd_start_addr, &initrd_end_addr);


    if (ret_get_initrd_range < 0) {
        uart_puts("Failed to get Initrd address.\n");
        while(1) {};
    }

    const void *initrd_start = (const void *)initrd_start_addr;
    const void *initrd_end = (const void *)initrd_end_addr;

    /*int ret_initrd_list = initrd_list(initrd_start, initrd_end);
    if (ret_initrd_list < 0) {
        uart_puts("Failed to get Initrd list.\n");
        while(1) {};
    }

    int ret_initrd_cat = initrd_cat(initrd_start, initrd_end, "osc.txt");
    if (ret_initrd_cat < 0) {
        uart_puts("\nosc.txt :Archive Error Or No such file.\n");
        while(1) {};
    }*/

    uart_puts("\nStarting kernel ...\n");

    uart_puts("Hart ID: ");
    uart_hex(hartid);
    uart_puts("\n");

    uart_puts("FDT Addr: ");
    uart_hex((unsigned long)fdt);
    uart_puts("\n");

    uart_puts("UART Base Addr: ");
    uart_hex((unsigned long)detected_uart_base);
    uart_puts("\n");

    uart_puts("Initrd Start Addr: ");
    uart_hex((unsigned long)initrd_start_addr);
    uart_puts("\n");

    uart_puts("Initrd End Addr: ");
    uart_hex((unsigned long)initrd_end_addr);
    uart_puts("\n");

    uart_puts("Initrd Size: ");
    uart_hex((unsigned long)initrd_end_addr - initrd_start_addr);
    uart_puts("\n");

    uart_puts("SBI specification version: ");
    uart_hex(sbi_get_spec_version());
    uart_puts("\n");

    while (1) {
        char buffer[64];

        uart_puts("opi-rv2> ");
        
        to_buffer(buffer, sizeof(buffer));
        command_parser(buffer, initrd_start, initrd_end);
    }
}

