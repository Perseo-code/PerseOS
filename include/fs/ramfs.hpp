#pragma once
#include <fs/fsnode.hpp>
#include <drivers/vga/vga.hpp>
#include <memory.hpp>
#include <stack.hpp>
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
        //print("kmalloc returned: ");
        //print(hexToString((uint32_t)node));
        //print("\n");
        for (int i = 0; i < NAMESIZE; i++) {
            node->name[i] = n[i];
        }
        node->type = t;
        node->parent = p;
        node->firstChild = f;
        node->nextSibling = ns;
        node->size = s;
        return node;
    }
    void mkdir(const char* n) {
        /*print("root = ");
        print(hexToString((uint32_t)root));
        print("\ncurrent = ");
        print(hexToString((uint32_t)current));
        print("\n");*/
        print("Creating folder: ");
        print(n);
        print("\n");
        /*if (current == nullptr) {
            print("current is nullptr\n");
        }
        else {
            print("current OK\n");
        }*/
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
    void ls(const char* n) {
        //print("this = ");
        //print(hexToString((uint32_t)this));
        if (current->firstChild == nullptr) {
            print("(empty)\n");
            return;
        }
        FSNode* last = current->firstChild;
        while (last) {
            print(last->name);
            if (last->type == Folder) {print("/");}
            print("\n");
            last = last->nextSibling;
        }
    }

    void cd(const char* n) {
        if (current->parent != nullptr && streq(n, "..")) {
            current = current->parent;
            return;
        }

        FSNode* last = current->firstChild;
        while (last) {
            if (streq(last->name, n) && last->type == Folder) {
                current = last;
                return;
            }

            if (streq(last->name, n) && last->type == File) {
                print("<Err:> '");
                print(n);
                print("' Is not a directory");
                print("\n");
                return;
            }
            last = last->nextSibling;
        }

        print("<Err:>There's no such directory as: ");
        print(n);
        print("\n");
    }

    void pwd() {
        if (current->parent == nullptr) {
            print("/");
            return;
        }

        Stack<FSNode*, 64> dirs;
        FSNode* dir = current;
        while (dir->parent != nullptr) {
            dirs.push(dir);
            dir = dir->parent;
        }

        while (!dirs.empty()) {
            FSNode* printme = dirs.pop();
            print("/");
            print(printme->name);
        }
        print("/");
        print("\n");
    }

    void create();
    FSNode* getCurrent() {
        return current;
    }
};

inline RamFS ramfs;