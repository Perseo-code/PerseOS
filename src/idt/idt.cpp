#include <idt/idt.hpp>

#define IRQ_LIST(X) \
    X(0)  X(1)  X(2)  X(3)  \
    X(4)  X(5)  X(6)  X(7)  \
    X(8)  X(9)  X(10) X(11) \
    X(12) X(13) X(14) X(15)
#define DECLARE_IRQ(n) extern "C" void irq##n();

IRQ_LIST(DECLARE_IRQ)

#undef DECLARE_IRQ

#define ISR_LIST(X) \
    X(0)  X(1)  X(2)  X(3)  \
    X(4)  X(5)  X(6)  X(7)  \
    X(8)  X(9)  X(10) X(11) \
    X(12) X(13) X(14) X(15) \
    X(16) X(17) X(18) X(19) \
    X(20) X(21) X(22) X(23) \
    X(24) X(25) X(26) X(27) \
    X(28) X(29) X(30) X(31)
#define DECLARE_ISR(n) extern "C" void isr##n();

ISR_LIST(DECLARE_ISR)

#undef DECLARE_ISR
#define SET_IRQ(n) idt_set_gate(32 + n, (uint32_t)irq##n, 0x08, 0x8E);
#define SET_ISR(n) idt_set_gate(n, (uint32_t)isr##n, 0x08, 0x8E);

IDTEntry idt[256];              

void idt_init() {
    // Configure idt's size
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;
    
    ISR_LIST(SET_ISR)
    #undef SET_ISR
    
    
    IRQ_LIST(SET_IRQ)
    #undef SET_IRQ

    // Load the idt on the CPU
    idt_load(&idtr);
}

void idt_set_gate(
    uint8_t vector,
    uint32_t handler,
    uint16_t selector,
    uint8_t flags)
{
    idt[vector].offset_low  = handler & 0xFFFF;
    idt[vector].selector    = selector;
    idt[vector].zero        = 0;
    idt[vector].flags       = flags;
    idt[vector].offset_high = handler >> 16;
}