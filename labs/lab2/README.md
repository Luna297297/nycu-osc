# NYCU OSC Lab2 — Booting

In Lab 2, I added three features to my freestanding RISC-V S-mode kernel:

1. Use the devicetree to find the UART and initial ramdisk.
2. Read files from a New ASCII CPIO archive.
3. Load another kernel over UART.

## Architecture Overview

```text
QEMU OpenSBI / OrangePi OpenSBI + U-Boot
                       │
                       │ loads the resident kernel
                       │ a0 = hart ID, a1 = DTB address
                       ▼
              Resident Loader Kernel
                       │
                       ├── discover the UART through FDT
                       ├── locate the initrd through /chosen
                       ├── provide ls/cat through the CPIO parser
                       └── run the interactive shell
                                    │
                                    │ load command
                                    ▼
                         UART Bootloader
                                    │
                         receive magic + size + payload
                                    │
                         write to a high memory address
                                    │
                                  fence.i
                                    │
                                    ▼
                    Uploaded High-Address Kernel
                       └── restart from its own _start
```

Both kernels are built from the same source code, but they have different roles:

- The resident kernel provides a stable UART loading environment.
- The uploaded kernel contains a newly built version under test.

## Supported Platforms

| Platform | Resident Kernel | Uploaded Kernel | UART Transport | Verified Features |
|---|---:|---:|---|---|
| QEMU virt | `0x80200000` | `0x82000000` | QEMU PTY (`/dev/pts/N`) | FDT, initrd/CPIO, shell, UART upload |
| OrangePi RV2 | `0x00200000` | `0x20000000` | Physical serial device | FDT, FIT ramdisk/CPIO, shell, UART upload |

## Build and Run

### QEMU

```bash
make BOARD=qemu run
make BOARD=qemu run-pty
make BOARD=qemu high-kernel
```

### OrangePi RV2

```bash
make BOARD=orangepi
cp kernel.bin board/kernel.bin

(
    cd board
    mkimage -f kernel.its kernel.itb
)

cp board/kernel.itb /path/to/mounted/boot/kernel.fit
sync

make BOARD=orangepi high-kernel
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
make BOARD=qemu run-pty
```

### Terminal 2: Read Kernel Output

```bash
cat /dev/pts/N
```

### Terminal 3: Upload the Kernel

```bash
make BOARD=qemu high-kernel
printf 'load\n' > /dev/pts/N
python3 tools/send.py /dev/pts/N high_kernel.bin
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

- Bootloader self-relocation from the Advanced Exercise is not implemented.
- Uploaded kernels run at `0x82000000` on QEMU and `0x20000000` on OrangePi RV2.
