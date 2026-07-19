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
        /*print("Creating folder: ");
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
            while (last->nextSibling != nullptr) {
                last = last->nextSibling;
                if (last->name == n) {
                    print("Folder already exists.");
                }
            }
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

    void create(const char* n) {
        if (current->firstChild == nullptr) {
            current->firstChild =  createNode(n, File, current, 0);
            return;
        }

        FSNode* last = current->firstChild;
        FSNode* before;
        while (last) {
            if (streq(last->name, n)) {
                print("File already exists. \n");
                return;
            }
            before = last;
            last = last->nextSibling;
        }
        FSNode* newFile = createNode(n, File, current, 0);
        before->nextSibling = newFile;
    }

    void gettype(const char* n) {
        if (n == nullptr) {
            print("Not a valid argument \n");
            return;
        }
        if (current->firstChild == nullptr) {
            print("File not found");
            return;
        }
        FSNode* s = current->firstChild;
        bool found = false;
        while (s) {
            if (streq(s->name, n)) {
                found = true;
                break;
            }
            s = s->nextSibling;
        }

        if (!found) {
            print(n);
            print(" does not exist\n");
            return;
        }

        switch (s->type) {
            case File:
                print(s->name);
                print("is a file\n");
                break;
            default:
                print(s->name);
                print("is a folder\n");
                break;
        }
    }
    FSNode* getCurrent() {
        return current;
    }
};

inline RamFS ramfs;