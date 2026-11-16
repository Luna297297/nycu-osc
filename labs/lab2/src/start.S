#include "memory_layout.h"

.section ".text.boot"
.global _start
_start:
#ifdef CONFIG_SELF_RELOCATE
    la t0, __image_start
    la t1, __image_end
    li t2, BOOTLOADER_RELOCATION_ADDR

relocate_loop:
    bgeu t0, t1, relocate_done
    lbu t3, 0(t0)
    sb t3, 0(t2)
    addi t0, t0, 1
    addi t2, t2, 1
    j relocate_loop

relocate_done:
    la t4, post_relocation
    la t5, __image_start
    sub t4, t4, t5

    li t5, BOOTLOADER_RELOCATION_ADDR
    add t4, t4, t5

    fence.i
    jr t4
#endif

post_relocation:
    la t0, __bss_start
    la t1, __bss_stop

loop_start:
    bgeu t0, t1, loop_end
    sd zero, 0(t0)
    addi t0, t0, 8
    j loop_start

loop_end:
    la sp, _end
    tail start_kernel
