#include <drivers/vga/vga.hpp>

static uint16_t y = 0;
static uint16_t x = 0;

void eraseRow(int row) {
    const char space = ' ';
    const uint8_t color = 0x00;
    uint16_t eraser = (color << 8) | space;
    for (int col = 0; col < VGA_WIDTH; col++) {
        VGA[row * VGA_WIDTH + col] = eraser;
    }
}

void scroll() {
    for (int row = 0; row < VGA_HEIGHT - 1; row++) {
        for (int column = 0; column < VGA_WIDTH; column++) {
            VGA[row * VGA_WIDTH + column] = VGA[(row + 1) * VGA_WIDTH + column];
        }
    }

    eraseRow(VGA_HEIGHT - 1);
}

void putchar(char character, uint8_t color)
{
    if (character == '\n') {
        y += 1;
        x = 0;
        VGA[y * 80 + x];
    } else {
        VGA[y * 80 + x] = (uint16_t(color) << 8) | character;
        x++;
    }
    if (x == VGA_WIDTH) {
        x = 0;
        if (y >= VGA_HEIGHT) {
            scroll();
            x = 0;
            y = VGA_HEIGHT - 1;
            return;
        }
        y++;
    }
    

    if (y >= VGA_HEIGHT) {
        scroll();
        x = 0;
        y = VGA_HEIGHT - 1;
        return;
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
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
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
    VGA[y * VGA_WIDTH + x] = eraser;
    if (x == 0) {
        x = VGA_WIDTH;
        y--;
    }
}

