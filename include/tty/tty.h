#pragma once
#include <vga/vga.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ascii/ascii.h>
#include <io/io.h>

void clear(void);
void putchar(char c);
void print(const char* text);
void input(char* buffer, int max_len);