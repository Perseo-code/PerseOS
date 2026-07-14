#pragma once
#include <stdint.hpp>

struct IDTEntry { // Similar to the GDT
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero; // Same structure as the segments
    uint8_t  flags;
    uint16_t offset_high;
} __attribute__((packed));

struct IDTR {
    uint16_t limit; // Table size in bytes - 1
    uint32_t base; // Lineal idt direction
} __attribute__((packed));


extern IDTEntry idt[256];
static IDTR idtr;
void idt_set_gate(
    uint8_t vector,
    uint32_t handler,
    uint16_t selector,
    uint8_t flags);


void idt_init();
extern "C" void idt_load(IDTR*);