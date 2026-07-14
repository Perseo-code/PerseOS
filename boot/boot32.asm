; We need this file to set up everything to enable long mode (64 bit)

BITS 32
section .text
global start32
extern kernel_main
start32:
   call kernel_main

.loop:
   hlt
   jmp .loop