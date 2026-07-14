#include <vga/vga.hpp>

void putchar(const char character, const uint8_t color) {
    uint16_t entry = (color << 8) | character;
    VGA[0] = entry;
}

void clean_screen() {
    /*We know that each character are 2 bytes. And the grid is 80x25
    So, we have to clean everything*/
}