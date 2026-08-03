#include "sbi.h"
#include "shell.h"
#include "string.h"
#include "uart.h"


void start_kernel() {
    uart_puts("\nStarting kernel ...\n");
    uart_puts("SBI specification version: ");
    uart_hex(sbi_get_spec_version());
    uart_puts("\n");

    uart_puts("Probe Set Timer: ");
    uart_hex(sbi_probe_extension(SBI_EXT_SET_TIMER));
    uart_puts("\n");

    uart_puts("Probe Shutdown:  ");
    uart_hex(sbi_probe_extension(SBI_EXT_SHUTDOWN));
    uart_puts("\n");
    while (1) {
        char buffer[64];

        uart_puts("opi-rv2> ");
        
        to_buffer(buffer, sizeof(buffer));
        command_parser(buffer);
    }
    
}

