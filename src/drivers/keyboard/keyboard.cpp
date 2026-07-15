#include <drivers/keyboard/keyboard.hpp>

static bool shiftPressed = false;


void readKey() {
    uint8_t scancode = inb(0x60);
    switch (scancode)
    {
        // Backspace
        case 14:
            eraseLast();
            return;
        // Left Shift pressed
        case 28:
            putchar('\n');
            return;
        case 42:
            shiftPressed = true;
            return;
        // Right Shift pressed
        case 54:
            shiftPressed = true;
            return;

        // Left Shift released
        case 170:
            shiftPressed = false;
            return;
        // Right Shift released
        case 182:
            shiftPressed = false;
            return;
    }
    
    if (scancode & 0x80) return;
    char c;
    switch (shiftPressed) {
        case true:
            c = shiftKeymap[scancode];
            break;
        default:
            c = keymap[scancode];
            break;
    }
    if (c)
        putchar(c);
}