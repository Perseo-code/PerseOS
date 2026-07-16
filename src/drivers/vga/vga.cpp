#include <drivers/vga/vga.hpp>

static uint16_t y = 0;
static uint16_t x = 0;

void putchar(char character, uint8_t color)
{
    if (character == '\n') {
        y += 1;
        x = 0;
        VGA[y * 80 + x];
        return;
    }
    
    VGA[y * 80 + x] = (uint16_t(color) << 8) | character;
    x++;

    if (x == 80) {
        x = 0;
        y++;
    }
}

void print(const string& text, const uint8_t color) {
    for (uint32_t i = 0; i < text.getSize(); i++) {
        putchar(text.get(i), color);
    }
}

void clean_screen() {
    /*We know that each character are 2 bytes. And the grid is 80x25
    So, we have to clean everything*/
    const char space = ' ';
    const uint8_t color = 0x00; // Black foreground
    uint16_t cleaner = (color << 8) | space;
    for (int i = 0; i < 80 * 25; i++) {
        VGA[i] = cleaner;
    }
    y = 0;
    x = 0;
}

void eraseLast() {
    const char space = ' ';
    const uint8_t color = 0x00;
    uint16_t eraser = (color << 8) | space;
    if (x == 0 && y == 0) {
        x = 0;
        y = 0;
        return;
    }
    x--;
    VGA[y * 80 + x] = eraser;
    if (x == 0) {
        x = 80;
        y--;
    }
}