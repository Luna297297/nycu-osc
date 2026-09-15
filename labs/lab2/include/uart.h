#ifndef UART_H
#define UART_H

#include <stdint.h>

/* =========================
 *  UART MMIO Registers
 * ========================= */

#if defined(CONFIG_BOARD_QEMU)
#define UART_LSR_OFFSET 0x05
typedef uint8_t uart_reg_t;
#elif defined(CONFIG_BOARD_ORANGEPI)
#define UART_LSR_OFFSET 0x14
typedef uint32_t uart_reg_t;
#else
#error "Unsupported board"
#endif

/* Line Status Register bits */
#define LSR_DR   (1U << 0)
#define LSR_TDRQ (1U << 5)

/* UART APIs */
char uart_getc(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_hex(unsigned long value);
void uart_init(uintptr_t base);

#endif
