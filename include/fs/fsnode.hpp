#pragma once
#include <stdint.hpp>

typedef enum {
    File,
    Folder
} Types;

class FSNode {
public:
    const char* name;
    Types type;
    FSNode* firstChild;
    FSNode* parent;
    FSNode* nextSibling;
    char* data;
    uint32_t size;
};