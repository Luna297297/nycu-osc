#ifndef UART_H
#define UART_H

#include <stdint.h>

/* =========================
 *  UART MMIO Registers
 * ========================= */

#if defined(CONFIG_BOARD_QEMU)
#define UART_BASE       0x10000000UL
#define UART_LSR_OFFSET 0x05
typedef uint8_t uart_reg_t;
#elif defined(CONFIG_BOARD_ORANGEPI)
#define UART_BASE       0xD4017000UL
#define UART_LSR_OFFSET 0x14
typedef uint32_t uart_reg_t;
#else
#error "Unsupported board"
#endif

#define UART_RBR ((volatile uart_reg_t *)(UART_BASE + 0x00))
#define UART_THR ((volatile uart_reg_t *)(UART_BASE + 0x00))
#define UART_LSR ((volatile uart_reg_t *)(UART_BASE + UART_LSR_OFFSET))

/* Line Status Register bits */
#define LSR_DR   (1U << 0)
#define LSR_TDRQ (1U << 5)

/* UART APIs */
char uart_getc(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_hex(unsigned long value);

#endif
