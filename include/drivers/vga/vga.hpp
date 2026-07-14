#pragma once
#include <stdint.hpp>
#include <string.hpp>
static volatile uint16_t* VGA = (volatile uint16_t*)0xB8000;

void putchar(const char, const uint8_t = 0x07);
void print(const string& text, const uint8_t = 0x07);
void clean_screen();