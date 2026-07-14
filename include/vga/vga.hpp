#pragma once
#include <stdint.hpp>
#include <string.hpp>
static volatile uint16_t* VGA = (volatile uint16_t*)0xB8000;

void putchar(const char, const uint8_t);
void print(string& text, const uint8_t);
void clean_screen();