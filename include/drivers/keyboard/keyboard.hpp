#pragma once
#include <drivers/vga/vga.hpp>
#include <io/io.hpp>

typedef void (*EnterCallback)(const char*);
void setEnterCallback(EnterCallback callback);

inline char inputBuffer[256];
inline uint32_t input_length = 0;
inline char commandBuffer[256];
inline uint32_t commandLength = 0;
const char keymap[128] = {
    0,
    27,                     // Esc
    '1','2','3','4','5','6','7','8','9','0',
    '\'',0,
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p',
    '`','+',
    '\n',
    0,                      // Ctrl
    'a','s','d','f','g','h','j','k','l',
    0,0,0,
    0,                      // Left Shift
    0,
    'z','x','c','v','b','n','m',
    ',', '.', '-',
    0,                      // Right Shift
    '*',
    0,                      // Alt
    ' ',
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

const char shiftKeymap[128] = {
    0,
    27,
    '!','"',0,'$','%','&','/','(',')','=',
    '?',0,
    '\b',
    '\t',
    'Q','W','E','R','T','Y','U','I','O','P',
    '^','*',
    '\n',
    0,
    'A','S','D','F','G','H','J','K','L',
    0,0,0,
    0,
    0,
    'Z','X','C','V','B','N','M',
    ';',':','_',
    0,
    '*',
    0,
    ' ',
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void readKey();