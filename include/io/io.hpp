/* ____/ PIC (Programmable Interrupts Controller) \________ */
/*
The PIC has two 8259 programmable interrupt controllers
/----------------|----------------\
|   Master PIC   |   Slave PIC    |
|----------------|----------------|
|  0x20 | 0x21   |  0xA0 | 0xA1   |
\----------------|----------------/

They generate interrupts by default
The problem is that interrupts 8-15 are CPU exceptions
So, we move them
*/

#pragma once
#include <stdint.hpp>

inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1"
        :
        : "a"(value), "Nd"(port));
}

inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile ("inb %1, %0"
        : "=a"(value)
        : "Nd"(port));
    return value;
}

constexpr uint8_t PIC1 = 0x20;
constexpr uint8_t PIC2 = 0xA0;

constexpr uint8_t PIC1_COMMAND = PIC1;
constexpr uint8_t PIC1_DATA    = PIC1 + 1;

constexpr uint8_t PIC2_COMMAND = PIC2;
constexpr uint8_t PIC2_DATA    = PIC2 + 1;

inline void io_wait()
{
    outb(0x80, 0);
}

void pic_remap(); // Changes the interrupt layout
void pic_sendEOI(uint8_t irq);