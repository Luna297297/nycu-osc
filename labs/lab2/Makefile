RISCV_GNU ?= riscv64-unknown-elf

CC      := $(RISCV_GNU)-gcc
LD      := $(RISCV_GNU)-ld
OBJCOPY := $(RISCV_GNU)-objcopy
QEMU    ?= qemu-system-riscv64

TARGET  := kernel.bin
ELF     := kernel.elf

BOARD ?= qemu

ifeq ($(BOARD),qemu)
LINKER := linker/qemu.ld
else ifeq ($(BOARD),orangepi)
LINKER := linker/orangepi.ld
else
$(error Unknown BOARD: $(BOARD))
endif

CFLAGS  := -mcmodel=medany -ffreestanding -nostdlib -g -Wall -Iinclude

build: clean
	$(CC) $(CFLAGS) -c src/*.S src/*.c
	$(LD) -T $(LINKER) -o $(ELF) *.o
	$(OBJCOPY) -O binary $(ELF) $(TARGET)

run: build
	$(QEMU) -M virt -m 256M -kernel $(TARGET) -display none -serial stdio

clean:
	rm -f $(TARGET) $(ELF) *.o

.PHONY: build run clean