RISCV_GNU ?= riscv64-unknown-elf

CC      := $(RISCV_GNU)-gcc
LD      := $(RISCV_GNU)-ld
OBJCOPY := $(RISCV_GNU)-objcopy
QEMU    ?= qemu-system-riscv64
INITRD  ?= ../../exercises/ex22/initramfs.cpio

TARGET  := kernel.bin
ELF     := kernel.elf

COMMON_C_SRCS := $(wildcard src/*.c)
COMMON_OBJS   := $(patsubst src/%.c,%.o,$(COMMON_C_SRCS))

BOOTLOADER_START_OBJ := bootloader_start.o
BOOTLOADER_ELF		 := bootloader.elf
BOOTLOADER_BIN		 := bootloader.bin

KERNEL_START_OBJ := kernel_start.o

PAYLOAD_OBJ    := payload_start.o
PAYLOAD_ELF    := payload.elf
PAYLOAD_BIN    := payload.bin
PAYLOAD_LINKER := linker/qemu_payload.ld

HIGH_KERNEL_OBJ := high_kernel_start.o
HIGH_KERNEL_ELF := high_kernel.elf
HIGH_KERNEL_BIN := high_kernel.bin

BOARD ?= qemu

ifeq ($(BOARD),qemu)
LINKER := linker/qemu.ld
HIGH_KERNEL_LINKER := linker/qemu_payload.ld
BOARD_CFLAGS := -DCONFIG_BOARD_QEMU
else ifeq ($(BOARD),orangepi)
LINKER := linker/orangepi.ld
HIGH_KERNEL_LINKER := linker/orangepi_payload.ld
BOARD_CFLAGS := -DCONFIG_BOARD_ORANGEPI
else
$(error Unknown BOARD: $(BOARD))
endif

CFLAGS  := -mcmodel=medany -ffreestanding -nostdlib -g -Wall -Iinclude $(BOARD_CFLAGS)

build: uploaded-kernel

payload:
	$(CC) $(CFLAGS) -c payload/start.S -o $(PAYLOAD_OBJ)
	$(LD) -T $(PAYLOAD_LINKER) -o $(PAYLOAD_ELF) $(PAYLOAD_OBJ)
	$(OBJCOPY) -O binary $(PAYLOAD_ELF) $(PAYLOAD_BIN)

high-kernel: build
	$(LD) -T $(HIGH_KERNEL_LINKER) -o $(HIGH_KERNEL_ELF) *.o
	$(OBJCOPY) -O binary $(HIGH_KERNEL_ELF) $(HIGH_KERNEL_BIN)

self-relocating-bootloader:
	rm -f *.o
	$(CC) $(CFLAGS) -c $(COMMON_C_SRCS)
	$(CC) $(CFLAGS) -DCONFIG_SELF_RELOCATE \
		-c src/start.S -o $(BOOTLOADER_START_OBJ)
	$(LD) -T $(LINKER) -o $(BOOTLOADER_ELF) \
		$(BOOTLOADER_START_OBJ) $(COMMON_OBJS)
	$(OBJCOPY) -O binary $(BOOTLOADER_ELF) $(BOOTLOADER_BIN)

uploaded-kernel:
	rm -f *.o
	$(CC) $(CFLAGS) -c $(COMMON_C_SRCS)
	$(CC) $(CFLAGS) -c src/start.S -o $(KERNEL_START_OBJ)
	$(LD) -T $(LINKER) -o $(ELF) \
		$(KERNEL_START_OBJ) $(COMMON_OBJS)
	$(OBJCOPY) -O binary $(ELF) $(TARGET)

run: build
	$(QEMU) -M virt -m 256M -kernel $(TARGET) -initrd $(INITRD) -display none -serial stdio
run-pty: build
	$(QEMU) -M virt -m 256M -kernel $(TARGET) -initrd $(INITRD) -display none -serial pty
clean:
	rm -f $(TARGET) $(ELF) \
	$(PAYLOAD_BIN) $(PAYLOAD_ELF) \
	$(HIGH_KERNEL_BIN) $(HIGH_KERNEL_ELF) \
	$(BOOTLOADER_BIN) $(BOOTLOADER_ELF) \
	*.o

.PHONY: build uploaded-kernel self-relocating-bootloader payload high-kernel run run-pty clean
