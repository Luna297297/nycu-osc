RISCV_GNU ?= riscv64-unknown-elf

CC      := $(RISCV_GNU)-gcc
LD      := $(RISCV_GNU)-ld
OBJCOPY := $(RISCV_GNU)-objcopy
QEMU    ?= qemu-system-riscv64
INITRD  ?= ../../exercises/ex22/initramfs.cpio

TARGET  := kernel.bin
ELF     := kernel.elf

PAYLOAD_OBJ    := payload_start.o
PAYLOAD_ELF    := payload.elf
PAYLOAD_BIN    := payload.bin
PAYLOAD_LINKER := linker/qemu_payload.ld

BOARD ?= qemu

ifeq ($(BOARD),qemu)
LINKER := linker/qemu.ld
BOARD_CFLAGS := -DCONFIG_BOARD_QEMU
else ifeq ($(BOARD),orangepi)
LINKER := linker/orangepi.ld
BOARD_CFLAGS := -DCONFIG_BOARD_ORANGEPI
else
$(error Unknown BOARD: $(BOARD))
endif

CFLAGS  := -mcmodel=medany -ffreestanding -nostdlib -g -Wall -Iinclude $(BOARD_CFLAGS)

build: clean
	$(CC) $(CFLAGS) -c src/*.S src/*.c
	$(LD) -T $(LINKER) -o $(ELF) *.o
	$(OBJCOPY) -O binary $(ELF) $(TARGET)

payload:
	$(CC) $(CFLAGS) -c payload/start.S -o $(PAYLOAD_OBJ)
	$(LD) -T $(PAYLOAD_LINKER) -o $(PAYLOAD_ELF) $(PAYLOAD_OBJ)
	$(OBJCOPY) -O binary $(PAYLOAD_ELF) $(PAYLOAD_BIN)

run: build
	$(QEMU) -M virt -m 256M -kernel $(TARGET) -initrd $(INITRD) -display none -serial stdio
run-pty: build
	$(QEMU) -M virt -m 256M -kernel $(TARGET) -initrd $(INITRD) -display none -serial pty
clean:
	rm -f $(TARGET) $(ELF) $(PAYLOAD_BIN) $(PAYLOAD_ELF) *.o

.PHONY: build payload run run-pty clean
