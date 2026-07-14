#include "kernel.hpp"

void init() {
    clean_screen();
    idt_init();
    pic_remap();
    asm volatile ("sti"); // Enable interruptions
}

extern "C" void kernel_main() {
    init();
    string text = "Hello world";
    const uint8_t color = 0x07;
    print(text, color);
    while (true) {}
}