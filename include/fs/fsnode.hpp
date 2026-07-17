#pragma once
#include <stdint.hpp>
#define NAMESIZE 32
typedef enum {
    File,
    Folder
} Types;

struct FSNode {
    char name[NAMESIZE];
    Types type;
    FSNode* firstChild;
    FSNode* parent;
    FSNode* nextSibling;
    char* data;
    uint32_t size;
};