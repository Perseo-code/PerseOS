#pragma once
#include <fs/fsnode.hpp>
#include <memory.hpp>
class RamFS {
private:
    FSNode* root;
    FSNode* current;
public:
    void init() {
        root = createNode("/", Folder, nullptr, 0);
        current = root;
    }

    FSNode* createNode(const char* n, Types t, FSNode* p, uint32_t s, FSNode* f = nullptr, FSNode* ns = nullptr) {
        FSNode* node = (FSNode*)kmalloc(sizeof(FSNode));
        node->name = n;
        node->type = t;
        node->parent = p;
        node->firstChild = f;
        node->nextSibling = ns;
        node->size = s;
        return node;
    }
    void mkdir(const char* n) {
        FSNode* newDir = createNode(n, Folder, current, 0);
        
        if (current->firstChild == nullptr) {
            current->firstChild = newDir;
            return;
        } else {
            FSNode* last = current->firstChild;
            while (last->nextSibling != nullptr)
                last = last->nextSibling;
            last->nextSibling = newDir;
        }
    }
    void create();
    void ls();
};