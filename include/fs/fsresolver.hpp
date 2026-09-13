#pragma once
#include <string.hpp>
#include <fs/fsnode.hpp>
#include <memory.hpp>

struct ParentResult {
    FSNode* parent;
    char name[NAMESIZE];
};

// Ram filesystem
FSNode* resolvePath(const char *path, FSNode* root, FSNode* current);
ParentResult resolveParent(const char* path, FSNode* root, FSNode* current, bool &err);

// Hard disk's filesystem
struct PFSParentResult { 
    char name[MAX_NAMESIZE];
    PFSNode* parent;
    uint32_t id;
    bool err;
};

PFSNode* resolvePFSPath(const char* path, PFSNode* root, PFSNode* current);
PFSParentResult resolvePFSParent(const char* path, PFSNode* root, PFSNode* current);