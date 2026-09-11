#include <fs/pfs.hpp>
#include <memory.hpp>
using namespace FS;

PFSNode* FS::createPFSNode(Types type, const char* name, bool extension, PFSNode* child = nullptr) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    result->type = type;
    strcpy(result->name, name);
    result->extension = extension;
    result->child = child;
    return result;
}

uint8_t* FS::encodePFSNode(PFSNode* node) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    size_t length = node->name_size;
    int i = 0;
    for (; i < length; i++) {
        result[i] = (uint8_t)node->name[i];
    }

    result[i] = node->type << 1; 
    result[i] |= node->extension;
    i++;
    *((size_t*)(result + i)) = node->name_size;
    i++;
    *((size_t*)(result + i)) = node->file_size;
    return result;
}

PFSNode* decodePFSNode(const uint8_t* metadata) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    int i = 0;
    while (i < MAX_NAMESIZE) {
        result->name[i] = metadata[i];
    }
}