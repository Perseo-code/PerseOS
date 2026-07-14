/*#pragma once
#include <stdint.hpp>

struct idt_entry_struct {
    uint16_t base_lo; // Offset 0-15
    uint16_t sel; // Segment selector
    uint8_t always0; // Always Zero. Literally
    uint8_t flags; // Types and privileges
    uint16_t base_hi; // Offset 16-31
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit; // Table size in bytes - 1
    uint32_t base; // Lineal idt direction
} __attribute__((packed));

extern void idt_load(uint32_t);

idt_ptr_struct idt_ptr;
idt_entry_struct idt_entry;
void idt_init();
*/