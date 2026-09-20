#include <timer/timer.hpp>
#include <io/io.hpp>
volatile uint32_t globalTime = 0;
volatile uint32_t localTimer = 0;
void PIT::init(uint16_t frequency) {
    constexpr uint32_t PIT_FREQ = 1193182;
    // Relationship: PIT frequency = 1,193,182 / divisor
    if (frequency == 0) return;
    uint32_t divisor = PIT_FREQ / frequency;
    if (divisor < 1) {
        divisor = 1;
    }
    if (divisor > 0xFFFF)
        divisor = 0xFFFF;
    /*NOTE: How to compose a PIT configure byte
    Channel: 0
    Access mode: low byte, then high byte
    Mode: 3 (square-wave generator)
    Counting: binary*/
    outb(0x43, 0x36); // Gives us 0x36
    // Low byte first, high byte last
    outb(0x40, (divisor & 0xFF));
    outb(0x40, (divisor >> 8) & 0xFF);
    // Unmask the PIT
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 0);       // Clear IRQ0 mask bit
    outb(0x21, mask);
}

void Time::millis() {
    globalTime++;
    localTimer++;
}

uint32_t Time::seconds() {
    return globalTime / 1000;
}

uint32_t Time::localSeconds() {
    return localTimer / 1000;
}

uint32_t Time::getGlobalTimer() {
    return globalTime;
} 

uint32_t Time::getTimer() {
    return localTimer;
}

void Time::resetTimer() {
    localTimer = 0;
}