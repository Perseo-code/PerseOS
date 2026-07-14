/*#include <idt/idt.hpp>

void idt_init() {
    // Configure idt's size
    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt_entry;

    // Load the idt on the CPU
    idt_load((uint32_t)&idt_ptr);
}*/