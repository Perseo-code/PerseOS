#include <timer/timer.h>
uint32_t system_time = 0;  // Current time in milliseconds

// Set the PIT frequency
void timer_init(void) {
    // The PIT timer is at I/O port 0x43, and we want to set its frequency
    // The PIT operates at a frequency of 1.19318 MHz by default

    // Set up the PIT control register
    outb(0x43, 0x36);  // Command: 0x36 sets mode 3 (square wave generator), 16-bit

    // Set the PIT frequency (1193180 Hz)
    uint32_t divisor = TIMER_FREQUENCY / 1000;  // 1000 Hz for 1 ms ticks
    outb(0x40, (uint8_t)(divisor & 0xFF));       // Low byte of divisor
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // High byte of divisor

    // Register the timer interrupt handler
    register_interrupt_handler(IRQ0, timer_interrupt_handler);
}

// This function returns the system time in milliseconds
uint32_t get_current_time(void) {
    return system_time;
}

// Timer interrupt handler (increments the system time every 1 ms)
void timer_interrupt_handler(void) {
    system_time++;  // Increment the system time every timer tick
}