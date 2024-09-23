bits 32 ; 32 bit mode
global _start
    section .text
        align 4 ;magic
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002 + 0x00)
        
    extern kernel_main
        
    _start:
        cli
        mov esp, stack_space
        call kernel_main
        hlt
    section .bss
    resb 8 * 2048 ; 16kb for stack
    stack_space:  