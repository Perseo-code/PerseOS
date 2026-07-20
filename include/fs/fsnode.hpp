#pragma once
#include <stdint.hpp>
#include <stddef.hpp>
#define NAMESIZE 32
constexpr size_t MAX_FILE_SIZE = 4096; // 4 KiB
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