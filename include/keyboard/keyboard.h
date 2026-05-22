#pragma once
#include <stdint.h>
#include <io/io.h>
#include <stdbool.h>
#include <ascii/ascii.h>
#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64
#define BUFFER_SIZE     64
#define DEBOUNCE_DELAY 200 // 200ms debounce time
#define REPEAT_DELAY 1000      // Time before starting to repeat (in ms)
#define REPEAT_INTERVAL 200    // Time between repetitions (in ms)

void keyboard_irq_handler(void);

// --- Read a scancode from the internal buffer ---
// Returns 0 if buffer is empty.
uint8_t keyboard_read_scancode(void);

// --- Get a single ASCII character ---
// Converts scancode to ASCII using Shift and CapsLock states.
char keyboard_getchar(void);


