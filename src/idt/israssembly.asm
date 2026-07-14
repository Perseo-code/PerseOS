BITS 32

%macro ISR 2
global isr%1
extern %2

isr%1:
    cli
    call %2
.hang:
    hlt
    jmp .hang
%endmacro

ISR 0,  isr_handler0
ISR 1,  isr_handler1
ISR 2,  isr_handler2
ISR 3,  isr_handler3
ISR 4,  isr_handler4
ISR 5,  isr_handler5
ISR 6,  isr_handler6
ISR 7,  isr_handler7
ISR 8,  isr_handler8
ISR 9,  isr_handler9
ISR 10, isr_handler10
ISR 11, isr_handler11
ISR 12, isr_handler12
ISR 13, isr_handler13
ISR 14, isr_handler14
ISR 15, isr_handler15
ISR 16, isr_handler16
ISR 17, isr_handler17
ISR 18, isr_handler18
ISR 19, isr_handler19
ISR 20, isr_handler20
ISR 21, isr_handler21
ISR 22, isr_handler22
ISR 23, isr_handler23
ISR 24, isr_handler24
ISR 25, isr_handler25
ISR 26, isr_handler26
ISR 27, isr_handler27
ISR 28, isr_handler28
ISR 29, isr_handler29
ISR 30, isr_handler30
ISR 31, isr_handler31