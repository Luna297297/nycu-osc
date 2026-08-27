.section ".text.boot"
.global _start
_start:
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
