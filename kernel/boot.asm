bits 32 ; 32 bit mode
global _start
    section .text
        align 4 ;magic, grub uses this
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002 + 0x00)
        
    extern kernel_main
        
    _start:
        cli
        mov esp, stack_space
        call kernel_main ; Jump to kernel
        hlt
    section .bss
    resb 8 * 4096 ; 32kb for stack
    stack_space: