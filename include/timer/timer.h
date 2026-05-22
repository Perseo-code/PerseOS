#pragma once

#include <stdint.h>  // For uint32_t
#include <idt/idt.h>
// Timer frequency in Hz (typically 18.2Hz for PIT)
#define TIMER_FREQUENCY  1193180  // PIT frequency

// Global variable to keep track of system time in milliseconds
extern uint32_t system_time;

// Function to initialize the timer
void timer_init(void);

// Function to get the current time in milliseconds
uint32_t get_current_time(void);

// Timer interrupt handler (to be called on every timer tick)
void timer_interrupt_handler(void);
