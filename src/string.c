#include <string.h>
#include <stddef.h>
#include <stdint.h>

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = dest;
    const uint8_t* s = src;
    while (n--) *d++ = *s++;
    return dest;
}

void* memset(void* s, int c, size_t n) {
    uint8_t* p = s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}

int memcmp(const void* a, const void* b, size_t n) {
    const uint8_t* p1 = a;
    const uint8_t* p2 = b;
    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++; p2++;
    }
    return 0;
}
