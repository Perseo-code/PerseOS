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