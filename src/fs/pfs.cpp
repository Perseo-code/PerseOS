#include <fs/pfs.hpp>
#include <memory.hpp>
using namespace FS;

PFSNode* FS::createPFSNode(Type type, const char* name, bool extension, PFSNode* parent = nullptr) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    result->type = type;
    strcpy(result->name, name);
    result->extension = extension;
    result->data = nullptr;
    result->parent = parent;
    return result;
}

uint8_t* FS::encodePFSNode(PFSNode* node) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    size_t length = strlen(node->name);
    int i = 0;
    for (; i < length; i++) {
        result[i] = (uint8_t)node->name[i];
    }

    result[i] |= node->type;
    result[i] |= node->extension;
    i++;
    length = strlen(node->data);
    for (; i < length; i++) {
        result[i] = (uint8_t)node->data[i];
    }

    return result;
}