#include <vga/vga.hpp>
#include <string.hpp>

extern "C" void divide_by_zero() {
    string exception = "\n Divide by Zero";
    uint8_t color = 0x07;
    print(exception, color);
}