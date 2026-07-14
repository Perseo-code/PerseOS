#pragma once
#include <stdint.hpp>
class string {
private:
    char* text;
    uint32_t length;
public:
    string(char* str) {
        length = 0;
        while (str[length] != '\0')
            length++;

        text = str;
    }

    // Constructor por defecto (para poder hacer: string miTexto;)
    string() : text("") {}

    string& operator=(char* newValue) { // We create this operator so when you do string name = "something", the text is saved in the text variable
        text = newValue;
        return *this;
    }

    uint32_t getSize() {return length;};
    char get(uint32_t index) {return text[index];}
    operator char*() const {
            return text; // To make the text represent the variable
    }
    
};