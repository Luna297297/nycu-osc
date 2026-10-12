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

typedef void (*payload_entry_t)(uint64_t hartid, const void *fdt);

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

        enum shell_action Status = command_parser(buffer, initrd_start, initrd_end);

        if (Status == SHELL_ACTION_LOAD) {
            int magic_idx = 0;
            uint32_t magic = 0;
            while (magic_idx < 4) {
                uint8_t byte = uart_getc_raw();

                magic |= (uint32_t) byte << (magic_idx * 8);
                magic_idx++;
            }
            
           uint32_t expected_magic = 0x544F4F42;

           if (magic != expected_magic) {
                uart_puts("Invalid boot magic\n");
                continue;
           } else {
                uart_puts("Boot magic validated.\n");
           }

           int kernel_size_idx = 0;
           uint32_t kernel_size = 0;
           while (kernel_size_idx < 4) {
                uint8_t byte = uart_getc_raw();

                kernel_size |= (uint32_t) byte << (kernel_size_idx * 8);
                kernel_size_idx++;
            }
            
            uart_puts("Kernel size: ");
            uart_hex(kernel_size);
            uart_puts("\n");
            
            uintptr_t des = 0x82000000UL;
            uintptr_t limit = initrd_start_addr;

            if (kernel_size == 0) {
                uart_puts("Invalid kernel size.\n");
                continue;
            }

            if (des >= limit) {
                uart_puts("Invalid kernel destination.\n");
                continue;
            }

            uintptr_t available = limit - des;

            if ((uintptr_t)kernel_size > available) {
                uart_puts("Kernel image is too large.\n");
                continue;
            }

            uart_puts("Kernel destination range validated.\n");

            // write kernel data
            uintptr_t write_data = des;
            uint32_t received = 0;

            while(received < kernel_size) {
                uint8_t *write_addr = (uint8_t *)write_data;
                *write_addr = uart_getc_raw();

                write_data++;
                received++;
            }

            uintptr_t read_addr = des;
            uint32_t read_cnt = 0;

            uart_puts("Data first 4 bytes:\n");
            while (read_cnt < 4 && read_cnt < kernel_size) {
                uint8_t *read_data = (uint8_t *)read_addr;
                uint8_t data = *read_data;

                uart_hex((unsigned long)data);
                uart_puts(" ");
                read_addr++;
                read_cnt++;
            }
            uart_puts("\n");

            asm volatile("fence.i" ::: "memory");
            payload_entry_t payload_entry = (payload_entry_t)des;
            payload_entry(hartid, fdt);

            uart_puts("Payload returned unexpectedly.\n");
            while (1) {
            }
        }

    }
}

