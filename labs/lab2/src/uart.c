#include "uart.h"

static uintptr_t uart_base;

#define UART_RBR ((volatile uart_reg_t *)(uart_base + 0x00))
#define UART_THR ((volatile uart_reg_t *)(uart_base + 0x00))
#define UART_LSR ((volatile uart_reg_t *)(uart_base + UART_LSR_OFFSET))


char uart_getc() {
    while ((*UART_LSR & LSR_DR) == 0)
        ;
    char c = (char)*UART_RBR;
    return c == '\r' ? '\n' : c;
}

void uart_putc(char c) {
    if (c == '\n')
        uart_putc('\r');

    while ((*UART_LSR & LSR_TDRQ) == 0)
        ;
    *UART_THR = c;
}

void uart_puts(const char* s) {
    while (*s)
        uart_putc(*s++);
}

void uart_hex(unsigned long h) {
    uart_puts("0x");
    unsigned long n;
    for (int c = 60; c >= 0; c -= 4) {
        n = (h >> c) & 0xf;
        n += n > 9 ? 0x57 : '0';
        uart_putc(n);
    }
}

void uart_init(uintptr_t base) {
    uart_base = base;
}