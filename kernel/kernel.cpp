#include "kernel.hpp"

extern "C" void kernel_main() {
    clean_screen();
    idt_init();
    string text = "Hello world";
    const uint8_t color = 0x07;
    print(text, color);
    while (true) {}
}