#include <idt/idt.hpp>
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();
IDTEntry idt[256];

void idt_init() {
    // Configure idt's size
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;
    
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
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