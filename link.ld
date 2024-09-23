OUTPUT_FORMAT(elf32-i386)
/* kernel/linker.ld */
ENTRY(_start)
SECTIONS
{
    . = 0x100000; /* Load address */

    .text :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    .rodata :
    {
        *(.rodata)
    }
}