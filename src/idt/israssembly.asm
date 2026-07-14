BITS 32
extern isr_handler
%macro ISR 1
global isr%1

isr%1:
    cli
    push dword %1
    call isr_handler
    add esp, 4

%%hang:
    hlt
    jmp %%hang
%endmacro

%assign i 0
%rep 32
ISR i
%assign i i+1
%endrep