#pragma once
#include <stdint.hpp>
class string {
private:
    const char* text;
    uint32_t length;
public:
    string(const char* str) {
        length = 0;
        while (str[length] != '\0')
            length++;

        text = str;
    }

    string() : text(""), length(0) {}

    string& operator=(const char* newValue) { // We create this operator so when you do string name = "something", the text is saved in the text variable
            text = newValue;

            length = 0;
            while (newValue[length] != '\0')
                length++;

            return *this;
    }

    uint32_t getSize() const {return length;};
    char get(uint32_t index) const {return text[index];}
    operator const char*() const {
            return text; // To make the text represent the variable
    };
    
};