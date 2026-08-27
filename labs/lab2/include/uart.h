#ifndef UART_H
#define UART_H

#include <stdint.h>

/* =========================
 *  UART MMIO Registers
 * ========================= */

#define UART_BASE 0xD4017000UL

/*
 * SpacemiT K1 UART
 * Register width : 32-bit
 * LSR offset     : 0x14
 */

#define UART_RBR ((volatile uint32_t *)(UART_BASE + 0x00))
#define UART_THR ((volatile uint32_t *)(UART_BASE + 0x00))
#define UART_LSR ((volatile uint32_t *)(UART_BASE + 0x14))

/* Line Status Register bits */
#define LSR_DR   (1U << 0)
#define LSR_TDRQ (1U << 5)

/* UART APIs */
char uart_getc(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_hex(unsigned long value);

#endif