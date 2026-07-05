section .bss
stack_bottom: resb 16384
stack_top:
section .text
global _start
_start:
    mov esp, stack_top
    jmp start32