#include <drivers/keyboard/keyboard.hpp>

void readKey() {
    uint8_t scancode = inb(0x60);

    if (!(scancode & 0x80))
    {
        char c = keymap[scancode];

        if (c)
            putchar(c);
    }
}