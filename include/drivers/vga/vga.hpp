#pragma once
#include <stdint.hpp>
#include <string.hpp>
#define VGA_HEIGHT 25
#define VGA_WIDTH 80

static volatile uint16_t* VGA = (volatile uint16_t*)0xB8000;

namespace VIDEO {
    void putchar(const char, const uint8_t = 0x07);
    void print(const string& text, const uint8_t = 0x07);
    void eraseLast();
    void eraseRow(int row);
    void clean_screen();
}