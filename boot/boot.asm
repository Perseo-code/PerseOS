section .bss
stack_bottom: resb 16384
stack_top:
section .text
global _start
extern kernel_main

_start:
    mov rsp, stack_top
    call kernel_main