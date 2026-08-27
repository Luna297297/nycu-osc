#include "sbi.h"
#include "shell.h"
#include "string.h"
#include "uart.h"

void to_buffer(char buffer[], int size) {
    int idx = 0;
    char c;

    while (idx < size - 1) {
        c = uart_getc();
        if (c == '\n') {
            uart_putc('\n');
            break;
        }

        uart_putc(c); //Echo

        buffer[idx] = c;
        idx++;
    }
    buffer[idx] = '\0';

    while (c != '\n') {
        c = uart_getc(); //如果 buffer 已滿，處理剩下多的 input
    }
}

void cmd_hello() {
    uart_puts("Hello World.\n");
}

void cmd_help() {
    uart_puts("Available commands:\n");
    uart_puts("  help - show all commands.\n");
    uart_puts("  hello - print Hello World.\n");
    uart_puts("  info - print system info.\n");
}

void cmd_info() {
    uart_puts("\nSystem information:\n");
    uart_puts("  OpenSBI specification version: ");
    uart_hex(sbi_get_spec_version());
    uart_puts("\n  Implementation ID: ");
    uart_hex(sbi_get_imp_id());
    uart_puts("\n  Implementation version: ");
    uart_hex(sbi_get_imp_version());
    uart_puts("\n");
}

void command_parser(char buffer[]) {
    if (strcmp(buffer, "hello") == 0) {
        cmd_hello();
    } else if (strcmp(buffer, "help") == 0) {
        cmd_help();
    } else if (strcmp(buffer, "info") == 0) {
        cmd_info();
    } else {
        uart_puts("Unknown command: ");
        uart_puts(buffer);
        uart_puts("\nUse help to get commands.\n");
    }
}