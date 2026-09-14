#pragma once
#include <stdint.hpp>
#include <stddef.hpp>
#include <drivers/ATA/ATA.hpp>
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
constexpr uint32_t SECTOR_SIZE = 512;
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
    bool children_loaded;
    bool has_children;
    PFSNode* parent;
    PFSNode* child;
    PFSExtension* extension_child;
    PFSNode* nextSibling;
    uint32_t parent_sector;
    uint32_t child_sector;
    uint32_t sibling_sector;
};

struct PFSExtension { 
    PFSNode* parent;
    uint32_t parent_sector;
    uint32_t sibling_sector;
    PFSExtension* sibling;
    char* content;
};

FSNode *createNode(const char *n, Types t, FSNode *p, uint32_t s, char *d = nullptr, FSNode *f = nullptr, FSNode *ns = nullptr);
void destroyNode(FSNode *node);
FSNode *findNode(const char *name, FSNode* current, FSNode *dir = nullptr, bool recursive = false);
FSNode *cloneNode(FSNode *node, FSNode *parent);
bool appendChild(FSNode* parent, FSNode* child);
PFSNode* createPFSNode(Types, const char*, PFSNode* parent, bool);
uint8_t* encodePFSNode(PFSNode* node);
PFSNode* decodePFSNode(const uint8_t* metadata);
PFSNode* findPFSNode(const char* n, PFSNode* dir, ATA* disk, bool recursive = false);
void loadPFSNode(PFSNode* parent, ATA* disk); 
bool appendPFSChild(PFSNode* parent, PFSNode* child);