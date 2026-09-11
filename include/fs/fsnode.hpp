#pragma once
#include <stdint.hpp>
#include <stddef.hpp>
#define NAMESIZE 32
constexpr size_t MAX_FILE_SIZE = 4096; // 4 KiB
typedef enum Types : bool {
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

constexpr uint8_t MAX_NAMESIZE = 255;

struct PFSNode {
    char name[MAX_NAMESIZE];
    size_t name_size;
    size_t file_size;
    Types type;
    bool extension;
    PFSNode* child;
};

struct PFSExtension { 
    PFSNode* parent;
    PFSExtension* sibling;
    char* content;
};