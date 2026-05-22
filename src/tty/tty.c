#include <tty/tty.h>

static size_t row = 0;
static size_t col = 0;
static vga_color fgcolor = VGA_WHITE;
static vga_color bgcolor = VGA_BLACK;

// Scrolls the screen in case that it's full
static void scroll(void) {
    uint8_t color = vga_entry_color(fgcolor, bgcolor);

    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA[(y - 1) * VGA_WIDTH + x] = VGA[y * VGA_WIDTH + x];
        }
    }

    // Clean the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        VGA[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', color);
    }

    if (row > 0) row--;
}

// Clears the screen
void clear(void) {
    uint8_t color = vga_entry_color(fgcolor, bgcolor);
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            VGA[y * VGA_WIDTH + x] = vga_entry(' ', color);
    row = 0;
    col = 0;
}

// Prints a character using the scroll function just in case
void putchar(char c) {
    uint8_t color = vga_entry_color(fgcolor, bgcolor);

    if (c == '\n') {
        col = 0;
        row++;
    } else if (c == '\r') {
        col = 0;
    } else {
        VGA[row * VGA_WIDTH + col] = vga_entry(c, color);
        col++;
        if (col >= VGA_WIDTH) {
            col = 0;
            row++;
        }
    }

    if (row >= VGA_HEIGHT) { // auto scroll
        scroll();
        row = VGA_HEIGHT - 1;
    }
}


// Prints a string with scroll (Scroll is in the putchar function)
void print(const char* str) {
    for (size_t i = 0; str[i]; i++)
        putchar(str[i]);
}





// input with echo & scroll
void input(char* buffer, int max_len) {
    int i = 0;
    char c;

    while (i < max_len - 1) {
        c = keyboard_getchar();
        if (c == '\r') break;
        putchar(c);
        buffer[i++] = c;
    }

    buffer[i] = '\0';
    putchar('\n');
}