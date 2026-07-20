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
        //print("RamFS::init()\n");

        root = createNode("/", Folder, nullptr, 0);

        /*if (root == nullptr)
            print("root is nullptr\n");
        else
            print("root OK\n");
        */
        current = root;

        /*if (current == nullptr)
            print("current is nullptr\n");
        else
            print("current OK\n");
        */
    }

    FSNode* createNode(const char* n, Types t, FSNode* p, uint32_t s, char* d = nullptr, FSNode* f = nullptr, FSNode* ns = nullptr) {
        FSNode* node = (FSNode*)kmalloc(sizeof(FSNode));
        //print("kmalloc returned: ");
        //print(hexToString((uint32_t)node));
        //print("\n");
        for (int i = 0; i < NAMESIZE; i++) {
            node->name[i] = n[i];
        }

        node->data = nullptr;

        if (d != nullptr) {
            uint32_t len = strlen(d);
            node->data = (char*)kmalloc(len + 1);

            if (node->data != nullptr) {
                strcpy(node->data, d);
                node->size = len;
            }
        }
        node->type = t;
        node->parent = p;
        node->firstChild = f;
        node->nextSibling = ns;
        node->size = s;
        return node;
    }

    void destroyNode(FSNode* node) {
        FSNode* parent = node->parent;
        if (parent->firstChild == node) {
            parent->firstChild = node->nextSibling;
        }

        FSNode* prev = parent->firstChild;

        while (prev && prev->nextSibling != node)
            prev = prev->nextSibling;

        if (prev)
            prev->nextSibling = node->nextSibling;
        
        if (node->data != nullptr) {
            kfree(node->data);
        }

        kfree(node);
    }

    FSNode* findNode(const char* name) {
        FSNode* node;
        if (current->firstChild == nullptr) {
            return nullptr;
        }

        if (current->firstChild != nullptr && streq(current->firstChild->name, name)) {
            return current->firstChild;
        }

        while (node) {
            if (streq(node->name, name)) {
                break;
            }
            node = node->nextSibling;
        }

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
        */
        /*if (current == nullptr) {
            print("current is nullptr\n");
        }
        else {
            print("current OK\n");
        }*/
        

        FSNode* newDir = createNode(n, Folder, current, 0);
        
        /*if (newDir == current)
            print("newDir == current\n");
        else
            print("newDir != current\n");*/
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
                print("File or Folder already exists. \n");
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
        FSNode* s = findNode(n);

        if (s == nullptr) {
            print(n);
            print(" does not exist\n");
            return;
        }

        switch (s->type) {
            case File:
                print(s->name);
                print(" is a file\n");
                break;
            default:
                print(s->name);
                print(" is a folder\n");
                break;
        }
    }

    void write(const char* filename, const char* content, bool overwrite = true) {
        if (filename == nullptr || content == nullptr) {
            print("No valid arguments \n");
            return;
        }

        if (current->firstChild == nullptr) {
            print(filename);
            print(" does not exist\n");
            return;
        }

        FSNode* fileToModify = findNode(filename);
        
        if (fileToModify == nullptr) {
            print(filename);
            print(" does not exist\n");
            return;
        }
        uint32_t len = 0;
        if (overwrite) {
            if (fileToModify->data != nullptr) {
                kfree(fileToModify->data);
            }
            len = strlen(content);
            fileToModify->data = (char*)kmalloc(len + 1);
            /*print("data ptr: ");
            print(hexToString((uint32_t)fileToModify->data));
            print("\n");

            print("content ptr: ");
            print(hexToString((uint32_t)content));
            print("\n");*/
            if (fileToModify->data == nullptr) {
                print("Out of memory\n");
            }
            strcpy(fileToModify->data, content);
            fileToModify->size = len;
            /*print("direct read: ");
            print(fileToModify->data);
            print("\n");*/

        } else {
            uint32_t oldLen = 0;

            if (fileToModify->data != nullptr) {
                oldLen = strlen(fileToModify->data);
            }

            uint32_t newLen = oldLen + strlen(content);

            char* temp = (char*)kmalloc(newLen + 1);

            if (temp == nullptr) {
                print("Out of memory\n");
                return;
            }

            temp[0] = '\0'; // important if old data is empty

            if (fileToModify->data != nullptr) {
                stradd(temp, fileToModify->data, content);
                kfree(fileToModify->data);
            } else {
                strcpy(temp, content);
            }

            fileToModify->data = temp;
            fileToModify->size = newLen;
        }
    }

    void read(const char* filename) {
        if (filename == nullptr) {
            print("No valid arguments \n");
            return;
        }

        if (current->firstChild == nullptr) {
            print(filename);
            print(" does not exist\n");
            return;
        }

        FSNode* fileToRead = findNode(filename);
        
        if (fileToRead->type != File) {
            print(filename);
            print(" is not a file");
            return;
        }
        if (fileToRead == nullptr) { 
            print(filename);
            print(" does not exist");
            return;
        }
        print(fileToRead->data);
        print("\n");
    }

    void size(const char* filename) {
        if (filename == nullptr) {
            print("Not a valid argument\n");
            return;
        }

        if (current->firstChild == nullptr) {
            print(filename);
            print(" does not exist");
            return;
        }

        FSNode* fileToRead = current->firstChild;
        while (fileToRead) {
            if (streq(fileToRead->name, filename) && fileToRead->type == File) {
                break;
            }
            fileToRead = fileToRead->nextSibling;
        }

        if (fileToRead == nullptr) {
            print(filename);
            print(" does not exist");
            return;
        }

        print("Current size of ");
        print(fileToRead->name);
        print(": ");
        print(intToString(fileToRead->size));
        print("\n");
        print("Unused bytes: ");
        print(intToString(4096 - fileToRead->size));
        print("\n");

    }

    void rm(const char* filename) {
        if (filename == nullptr) {
            print("Not a valid argument\n");
            return;
        }

        FSNode* fileToRemove = findNode(filename);
        
        if (fileToRemove == nullptr) {
            print("File ");
            print(filename);
            print(" does not exist\n");
            return;
        }

        if (fileToRemove->type != File) {
            print(filename);
            print(" is not a file\n");
            return;
        }
        destroyNode(fileToRemove);
    }

    void rmdir(const char* dirname) {
        if (dirname == nullptr) {
            print("Not a valid agument\n");
            return;
        }

        FSNode* folderToRemove = findNode(dirname);
        if (folderToRemove == nullptr) {
            print("Folder ");
            print(dirname);
            print(" does not exist");
            return;
        }

        if (folderToRemove->type != Folder) {
            print(dirname);
            print(" is not a folder");
            return;
        }
        if (folderToRemove->firstChild != nullptr) {
            print(dirname);
            print(" is not empty!");
            return;
        }
        destroyNode(folderToRemove);
    }
    FSNode* getCurrent() {
        return current;
    }
};

inline RamFS ramfs;