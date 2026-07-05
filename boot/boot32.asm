; We need this file to set up everything to enable long mode (64 bit)

BITS 32

section .text
global start32

start32:
   cli ; First, we start off by clearing all the interruptions because we don't have an IDT. The CPU wouldn't know where to jump
   lgdt [gdt64_descriptor]

   ; Reload data segments
   mov ax, DATA_SELECTOR
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax

   ; Reload code segment
   jmp CODE_SELECTOR:flush

flush:
    ; Now we're executing using our own GDT.
   