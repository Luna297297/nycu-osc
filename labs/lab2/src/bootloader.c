#include "bootloader.h"
#include "uart.h"

typedef void (*payload_entry_t)(uint64_t hartid, const void *fdt);

static uint32_t uart_read_u32_le(void) {
    uint32_t value = 0;

    for (int i = 0; i < 4; i++) {
        uint8_t byte = uart_getc_raw();
        value |= (uint32_t)byte << (i * 8);
    }

    return value;
}

void bootloader_load(uint64_t hartid,
                     const void *fdt,
                     uintptr_t destination,
                     uintptr_t limit) {
    uint32_t magic = uart_read_u32_le();
    const uint32_t expected_magic = 0x544F4F42U;

    if (magic != expected_magic) {
        uart_puts("Invalid boot magic\n");
        return;
    }
    uart_puts("Boot magic validated.\n");

    uint32_t kernel_size = uart_read_u32_le();

    uart_puts("Kernel size: ");
    uart_hex(kernel_size);
    uart_puts("\n");

    if (kernel_size == 0) {
        uart_puts("Invalid kernel size.\n");
        return;
    }

    if (destination >= limit) {
        uart_puts("Invalid kernel destination.\n");
        return;
    }

    uintptr_t available = limit - destination;
    if ((uintptr_t)kernel_size > available) {
        uart_puts("Kernel image is too large.\n");
        return;
    }

    uart_puts("Kernel destination range validated.\n");

    uintptr_t write_address = destination;
    uint32_t received = 0;
    while (received < kernel_size) {
        uint8_t *write_pointer = (uint8_t *)write_address;
        *write_pointer = uart_getc_raw();

        write_address++;
        received++;
    }

    uintptr_t read_address = destination;
    uint32_t read_count = 0;

    uart_puts("Data first 4 bytes:\n");
    while (read_count < 4 && read_count < kernel_size) {
        const uint8_t *read_pointer = (const uint8_t *)read_address;
        uart_hex((unsigned long)*read_pointer);
        uart_puts(" ");

        read_address++;
        read_count++;
    }
    uart_puts("\n");

    asm volatile("fence.i" ::: "memory");

    payload_entry_t payload_entry = (payload_entry_t)destination;
    payload_entry(hartid, fdt);

    uart_puts("Payload returned unexpectedly.\n");
    while (1) {
    }
}
