BITS 32

section .bss
global stack_top
stack_bottom: resb 16384
stack_top:
section .text
%include "gdt.inc"
global _start
extern gdt_descriptor
extern start32
_start:
   cli
   mov esp, stack_top
   lgdt [gdt_descriptor]

   ; Reload data segments
   mov ax, DATA_SELECTOR
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax

   ; Finally, we boot the kernel
   jmp CODE_SELECTOR:start32