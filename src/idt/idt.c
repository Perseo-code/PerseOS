#include <idt/idt.h>

#define IDT_SIZE 256

idt_entry_t idt[IDT_SIZE];
idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = sel;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Initializes all entrys to 0
    for(int i = 0; i < IDT_SIZE; i++)
        idt_set_gate(i, 0, 0, 0);

    // Example: vector 0x80 -> syscall
    extern void syscall_handler();  // from syscallasm.asm
    idt_set_gate(0x80, (uint32_t)syscall_handler, 0x08, 0x8E);

    // Register IRQ0 (keyboard)
    extern void keyboard_irq_handler();
    idt_set_gate(0x21, (uint32_t)keyboard_irq_handler, 0x08, 0x08E  );

    idt_flush((uint32_t)&idt_ptr); // Loads the IDT on the CPU
}
