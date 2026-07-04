BITS 32

section .bss
stack_bottom: resb 16384
stack_top:
section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    ;call kernel_main
    mov word [0xB8000], 0x0741    ; White 'A

.loop:
    ; hlt
    jmp .loop