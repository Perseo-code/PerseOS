#include <ascii/ascii.h>

char scancode_to_ascii(uint8_t sc, bool shift, bool capslock) {
    // Resolve capslock key problem (Makes letters normal in case you're pressing caps and shift at the same time)
    if (!shift && capslock) {
        if (sc >= 0x10 && sc <= 0x19) return normal[sc] - 32; // q-p
        if (sc >= 0x1E && sc <= 0x26) return normal[sc] - 32; // a-l
        if (sc >= 0x2C && sc <= 0x32) return normal[sc] - 32; // z-m
    }

    if (shift) {
        return shifted[sc];
    } else {
        return normal[sc];
    }
}
