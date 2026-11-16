OUTPUT_ARCH(riscv)
ENTRY(_start)
SECTIONS
{
    . = 0x00200000;
    __image_start = .;
    _start = .;
    .text : { *(.text.boot) *(.text) *(.text.*)}
    .rodata : { *(.rodata) *(.rodata.*)}
    .data : { *(.data) *(.data.*) *(.sdata) *(.sdata.*)}
    __image_end = .;
    .bss : {
        __bss_start = .;
        *(.sbss) *(.sbss.*) *(.bss) *(.bss.*) *(COMMON)
        __bss_stop = .;
    }
    . = ALIGN(16);
    __stack_bottom = .;
    . = . + 0x4000;
    __stack_top = .;
    __runtime_end = .;
    _end = .;
}
