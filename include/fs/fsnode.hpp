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
constexpr uint32_t PFS_MAGIC = 0xBADB01FF;
struct PFSSuperblock {
    uint32_t magic;
    uint32_t version;

    uint32_t root_lba;
    uint32_t total_sectors;

    bool valid;
};

struct PFSNode {
    char name[MAX_NAMESIZE];
    uint32_t name_size;
    uint32_t file_size;
    uint32_t sector;
    Types type;
    bool extension;
    PFSNode* child;
    uint32_t child_sector;
};

struct PFSExtension { 
    PFSNode* parent;
    uint32_t parent;
    uint32_t sibling;
    PFSExtension* sibling;
    char* content;
};