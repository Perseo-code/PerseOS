#include "kernel.hpp"

extern "C" void kernel_main() {
    const char letter = 'A';
    const uint8_t color = 0x07;
    putchar(letter, color);
    while (true) {}
}