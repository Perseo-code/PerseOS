#include <string.hpp>

const char* intToString(int value)
{
    static char buffer[12];

    int i = 0;
    bool neg = false;

    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (value < 0)
    {
        neg = true;
        value = -value;
    }

    while (value > 0)
    {
        buffer[i++] = '0' + value % 10;
        value /= 10;
    }

    if (neg)
        buffer[i++] = '-';

    buffer[i] = '\0';

    for (int j = 0; j < i / 2; j++)
    {
        char t = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = t;
    }

    return buffer;
}


const char* hexToString(uint32_t value)
{
    static char buffer[11]; // "0x" + 8 digits + '\0'

    const char* digits = "0123456789ABCDEF";

    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++)
    {
        buffer[9 - i] = digits[value & 0xF];
        value >>= 4;
    }

    buffer[10] = '\0';

    return buffer;
}

uint32_t strlen(const char* a) {
    uint32_t i = 0;
    while (a[i]) {
        i++;
    }

    return i;
}

char* strcpy(char* dest, const char* origin) {
    int i = 0;
    while (origin[i] != '\0') {
        dest[i] = origin[i];
        i++;
    }
    dest[i] = '\0';

    return dest;
}

void stradd(char* dest, const char* a, const char* b) {
    int i = 0;
    int j = 0;

    while (a[j] != '\0') {
        dest[i] = a[j];
        i++;
        j++;
    }

    j = 0; 

    while (b[j] != '\0') {
        dest[i] = b[j];
        i++;
        j++;
    }


    dest[i] = '\0';
}


bool streq(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return false;

        a++;
        b++;
    }

    return *a == *b;
}

extern "C" void* memset(void* dest, int val, unsigned long count) {
    unsigned char* ptr = (unsigned char*)dest;
    while (count-- > 0) {
        *ptr++ = (unsigned char)val;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}
