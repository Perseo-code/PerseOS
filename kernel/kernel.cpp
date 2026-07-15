#include "kernel.hpp"

void init() {
    clean_screen();
    idt_init();
    pic_remap();

    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
    asm volatile ("sti"); // Enable interruptions
}

extern "C" void kernel_main() {
    init();
    print("Welcome to PerseOS \n");

    while (true) {}
}