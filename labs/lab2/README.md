# NYCU OSC Lab2 — Booting

In Lab 2, I added four features to my freestanding RISC-V S-mode kernel:

1. Use the devicetree to find the UART and initial ramdisk.
2. Read files from a New ASCII CPIO archive.
3. Load another kernel over UART.
4. Relocate the resident bootloader before loading the new kernel.

## Architecture Overview

```text
QEMU OpenSBI / OrangePi OpenSBI + U-Boot
                       │
                       │ load at the standard address
                       │ a0 = hart ID, a1 = DTB address
                       ▼
               Resident Bootloader
                       │ copy text, rodata, and data
                       │ fence.i
                       ▼
              High-address Bootloader
                       │ clear BSS and set a new stack
                       │ initialize FDT, UART, and initrd
                       │ run the shell
                       ▼
                  load command
                       │ receive magic + size + payload
                       │ write to the standard address
                       │ fence.i
                       ▼
                 Uploaded Kernel
                       └── restart from its own _start
```

Both images are built from the same source code, but they have different roles:

- The resident bootloader provides a stable UART loading environment.
- The uploaded kernel contains a newly built version under test.

## Supported Platforms

| Platform | Standard Address | Relocated Bootloader | Uploaded Kernel | UART Transport |
|---|---:|---:|---:|---|
| QEMU virt | `0x80200000` | `0x82000000` | `0x80200000` | QEMU PTY (`/dev/pts/N`) |
| OrangePi RV2 | `0x00200000` | `0x20000000` | `0x00200000` | `/dev/ttyUSBx` |

## Build and Run

### QEMU

```bash
make self-relocating-bootloader BOARD=qemu
make uploaded-kernel BOARD=qemu

qemu-system-riscv64 \
    -M virt \
    -m 256M \
    -kernel bootloader.bin \
    -initrd ../../exercises/ex22/initramfs.cpio \
    -display none \
    -serial pty
```

### OrangePi RV2

```bash
make self-relocating-bootloader BOARD=orangepi
make uploaded-kernel BOARD=orangepi
cp bootloader.bin board/bootloader.bin

(
    cd board
    mkimage -f kernel.its kernel.itb
)

cp board/kernel.itb /path/to/mounted/boot/kernel.fit
sync
```

## UART Bootloader Demo

Protocol:

```text
+------------------+------------------+-----------------------+
| Magic: "BOOT"    | Size: uint32 LE  | Kernel payload        |
| 4 bytes          | 4 bytes          | size bytes            |
+------------------+------------------+-----------------------+
```

Use the `/dev/pts/N` path printed by QEMU in the following commands.

### Terminal 1: Start QEMU

```bash
qemu-system-riscv64 \
    -M virt \
    -m 256M \
    -kernel bootloader.bin \
    -initrd ../../exercises/ex22/initramfs.cpio \
    -display none \
    -serial pty
```

### Terminal 2: Read Kernel Output

```bash
cat /dev/pts/N
```

### Terminal 3: Upload the Kernel

```bash
printf 'load\n' > /dev/pts/N
python3 tools/send.py /dev/pts/N kernel.bin
```

Expected output:

```text
Boot magic validated.
Kernel destination range validated.

Starting kernel ...
UART Base Addr: 0x0000000010000000
opi-rv2>
```

## Known Limitations

- The relocation destination is selected by a board-specific memory policy instead of being discovered from the devicetree memory node.
- The UART protocol contains only the magic number, payload size, and payload. It does not provide a checksum or retransmission.
