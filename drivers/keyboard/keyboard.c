#include <keyboard/keyboard.h>

static uint8_t buffer[BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

// Variables de estado persistentes
static bool shift_active = false;
static bool capslock_active = false;

static void push(uint8_t sc) {
    uint8_t next = (head + 1) % BUFFER_SIZE;
    if (next != tail) {
        buffer[head] = sc;
        head = next;
    }
}

static uint8_t pop(void) {
    if (head == tail) return 0;
    uint8_t sc = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return sc;
}

void keyboard_irq_handler(void) {
    uint8_t sc = inb(0x60); // KBD_DATA_PORT
    
    // FILTRO CRÍTICO: 
    // En el Set 1, si el bit 7 está activo (sc >= 0x80), es que han SOLTADO la tecla.
    if (sc & 0x80) {
        // Aquí detectamos si soltaron Shift
        uint8_t released_sc = sc & 0x7F;
        if (released_sc == 0x2A || released_sc == 0x36) {
            shift_active = false;
        }
        return; // No guardamos en el buffer cuando se suelta una tecla
    }

    // Si llegamos aquí, es una pulsación (Make code)
    if (sc == 0x2A || sc == 0x36) {
        shift_active = true;
    } else if (sc == 0x3A) {
        capslock_active = !capslock_active;
    } else {
        push(sc); // Solo guardamos teclas "imprimibles" o especiales de presión
    }
}

char keyboard_getchar(void) {
    char c = 0;
    while (c == 0) {
        uint8_t sc = pop();
        if (sc == 0) {
            // Aquí podrías poner un "hlt" para no quemar la CPU, 
            // pero por ahora solo esperamos.
            continue; 
        }

        // Convertimos el scancode usando el estado que ya actualizó la IRQ
        c = scancode_to_ascii(sc, shift_active, capslock_active);
    }
    return c;
}
