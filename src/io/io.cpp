#include <io/io.hpp>
#include <drivers/keyboard/keyboard.hpp>
void pic_remap()
{
    uint8_t masterMask = inb(PIC1_DATA);
    uint8_t slaveMask  = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);
    io_wait();

    outb(PIC2_COMMAND, 0x11);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();

    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();

    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, 0x01);
    io_wait();

    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, masterMask);
    outb(PIC2_DATA, slaveMask);
}

void pic_sendEOI(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, 0x20);

    outb(PIC1_COMMAND, 0x20);
}

extern "C" void irq_handler(uint32_t irq) {
    if (irq == 1) {
        readKey();
    }

    pic_sendEOI(irq);
}