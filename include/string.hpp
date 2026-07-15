#pragma once
#include <stdint.hpp>
class string {
private:
    char text[256];
    uint32_t length;
public:
    string(const char* str) {
        *this = str;
    }

    string() : length(0) { text[0] = '\0'; }

    /*string& operator=(const char* newValue) { 
            text = newValue;

            length = 0;
            while (newValue[length] != '\0')
                length++;

            return *this;
    }*/

    string& operator=(const char* str)
    {
        length = 0;

        while (str[length] && length < sizeof(text) - 1) // We create this operator so when you do string name = "something", the text is saved in the text variable
        {
            text[length] = str[length];
            length++;
        }

        text[length] = '\0';
        return *this;
    }
    
    uint32_t getSize() const {return length;};
    
    char get(uint32_t index) const {return text[index];}
    
    string& join(const string& other)
    {
        while (length < sizeof(text) - 1 &&
            other.text[length - (this->length)] != '\0')
        {
            text[length] = other.text[length - this->length];
            length++;
        }

        text[length] = '\0';
        return *this;
    }

    operator const char*() const {
            return text; // To make the text represent the variable
    };
    
};
const char* intToString(int value);