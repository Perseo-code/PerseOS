#include <drivers/keyboard/keyboard.hpp>

static bool shiftPressed = false;
static EnterCallback enterCallback = nullptr;

void setEnterCallback(EnterCallback callback) {
    enterCallback = callback;
}

void readKey() {
    uint8_t scancode = inb(0x60);
    switch (scancode)
    {
        // Backspace
        case 14:
            eraseLast();
            return;
        // Enter pressed
        case 28:
            putchar('\n');
            commandBuffer[commandLength] = '\0';

            if (enterCallback)
                enterCallback(commandBuffer);
            
            commandLength = 0;

            return;
        // Left shift pressed
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
        inputBuffer[input_length++] = c;
        commandBuffer[commandLength++] = c;
        putchar(c);
}