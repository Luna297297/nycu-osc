.section ".text.boot"
.global _start
_start:
    mv s0, a0
    mv s1, a1


uart_wait:
    li t0, 0x10000000
    lbu t1, 5(t0)
    andi t1, t1, 0x20
    beqz t1, uart_wait
    li t1, 'P'
    sb t1, 0(t0)
    
    bnez s0, context_fail
    beqz s1, context_fail

    lbu t2, 0(s1)
    li t1, 0xd0
    bne t2, t1, context_fail

    lbu t2, 1(s1)
    li t1, 0x0d
    bne t2, t1, context_fail

    lbu t2, 2(s1)
    li t1, 0xfe
    bne t2, t1, context_fail

    lbu t2, 3(s1)
    li t1, 0xed
    bne t2, t1, context_fail

context_success:
    li t2, 'C'
    j context_uart_wait

context_fail:
    li t2, 'E'

context_uart_wait:
    lbu t1, 5(t0)
    andi t1, t1, 0x20
    beqz t1, context_uart_wait
    sb t2, 0(t0)

payload_loop:
    j payload_loop

