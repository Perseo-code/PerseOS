#pragma once
#include <stdint.h>
/*
idt.h: Interruption descriptor table.
With this, the CPU knows what function to execute.
Without this, int 0x80 wouldn't exist in assembly.
This is way harder to explain than any other module that i made.
*/

// Structures

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

// Functions
void idt_set_gate(uint8_t n, uint32_t handler, uint16_t sel, uint8_t flags);
void idt_init(void);
