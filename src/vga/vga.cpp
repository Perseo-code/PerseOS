#include <vga/vga.hpp>

void putchar(const char character, const uint8_t color) {
    uint16_t entry = (color << 8) | character;
    VGA[0] = entry;
}