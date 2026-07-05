BITS 64

section .bss
stack_bottom: resb 16384
stack_top:
section .text
global start64
extern kernel_main

start64:
    mov rsp, stack_top
    call kernel_main

.loop:
    ; hlt
    jmp .loop