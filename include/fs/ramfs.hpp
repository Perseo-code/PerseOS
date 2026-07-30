#pragma once
#include <fs/fsnode.hpp>
#include <drivers/vga/vga.hpp>
#include <memory.hpp>
#include <stack.hpp>
#include <string.hpp>
struct ParentResult {
    FSNode* parent;
    char name[NAMESIZE];
};

class RamFS
{
private:
    FSNode *root;
    FSNode *current;

public:
    void init()
    {
        // print("RamFS::init()\n");

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

    FSNode *createNode(const char *n, Types t, FSNode *p, uint32_t s, char *d = nullptr, FSNode *f = nullptr, FSNode *ns = nullptr)
    {
        FSNode *node = (FSNode *)kmalloc(sizeof(FSNode));
        // print("kmalloc returned: ");
        // print(hexToString((uint32_t)node));
        // print("\n");
        for (int i = 0; i < NAMESIZE; i++)
        {
            node->name[i] = n[i];
        }

        node->data = nullptr;

        if (d != nullptr)
        {
            uint32_t len = strlen(d);
            node->data = (char *)kmalloc(len + 1);

            if (node->data != nullptr)
            {
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

    void destroyNode(FSNode *node)
    {
        FSNode *parent = node->parent;
        if (parent->firstChild == node)
        {
            parent->firstChild = node->nextSibling;
        }

        FSNode *prev = parent->firstChild;

        while (prev && prev->nextSibling != node)
            prev = prev->nextSibling;

        if (prev)
            prev->nextSibling = node->nextSibling;

        if (node->data != nullptr)
        {
            kfree(node->data);
        }

        kfree(node);
    }

    FSNode *findNode(const char *name, FSNode *dir = nullptr, bool recursive = false)
    {
        if (dir == nullptr)
        {
            dir = current;
        }
        if (dir->firstChild == nullptr)
        {
            return nullptr;
        }

        FSNode *node = dir->firstChild;
        FSNode *foundit;
        while (node)
        {
            if (node->type == Folder && recursive)
            {
                foundit = findNode(name, node, true);
                if (foundit != nullptr)
                {
                    return foundit;
                }
            }
            if (streq(node->name, name))
            {
                return node;
            }
            node = node->nextSibling;
        }

        return nullptr;
    }

    FSNode *cloneNode(FSNode *node, FSNode *parent)
    {
        FSNode *copy =
            createNode(
                node->name,
                node->type,
                parent,
                node->size,
                node->data);

        if (node->type == Folder)
        {
            FSNode *child = node->firstChild;
            FSNode *lastCopy = nullptr;

            while (child)
            {
                FSNode *childCopy =
                    cloneNode(child, copy);

                if (copy->firstChild == nullptr)
                    copy->firstChild = childCopy;
                else
                    lastCopy->nextSibling = childCopy;

                lastCopy = childCopy;
                child = child->nextSibling;
            }
        }

        return copy;
    }

    bool appendChild(FSNode* parent, FSNode* child)
    {
        if (parent == nullptr || child == nullptr)
            return false;

        if (parent->firstChild == nullptr)
        {
            parent->firstChild = child;
            return true;
        }

        FSNode* last = parent->firstChild;

        while (last->nextSibling)
            last = last->nextSibling;

        last->nextSibling = child;
        return true;
    }
    void mkdir(const char *n)
    {
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
        bool err = false;
        ParentResult p = resolveParent(n, err);

        if (err) {
            print("resolveParent failed\n");
            return;
        }
        if (p.parent == nullptr)
        {
            print("resolveParent returned null parent\n");
            return;
        }
        FSNode* newDir = createNode(p.name, Folder, p.parent, 0);
        
        /*if (newDir == current)
            print("newDir == current\n");
        else
            print("newDir != current\n");*/
        if (p.parent->firstChild == nullptr)
        {
            p.parent->firstChild = newDir;
            return;
        }
        else
        {
            if (findNode(p.name, p.parent, false))
            {
                print("Folder already exists.\n");
                return;
            }

            appendChild(p.parent, newDir);
        }
    }

    FSNode* resolvePath(const char *path)
    {
        FSNode *node;

        if (path[0] == '/')
        {
            node = root;
            path++; // Skip the leading '/'
        }
        else
        {
            node = current;
        }

        char component[NAMESIZE];

        while (*path != '\0')
        {
            // Skip repeated slashes
            while (*path == '/')
                path++;

            if (*path == '\0')
                break;

            // Read one path component
            int i = 0;

            while (*path != '\0' && *path != '/')
            {
                if (i < NAMESIZE - 1)
                    component[i++] = *path;

                path++;
            }

            component[i] = '\0';
            if (streq(component, "."))
            {
                continue;
            }

            if (streq(component, ".."))
            {
                if (node->parent != nullptr)
                    node = node->parent;

                continue;
            }
            // Find this child inside the current node
            node = findNode(component, node, false);

            if (node == nullptr)
                return nullptr;
        }

        return node;
    }


    ParentResult resolveParent(const char* path, bool &err) {
        err = false;
        if (path == nullptr || *path == '\0') {
            err = true;
            return {};
        }

        FSNode *node;
        ParentResult result{};
        //print("err = ");
        //print(err ? "true\n" : "false\n");
        if (path[0] == '/')
        {
            node = root;
            path++; // Skip the leading '/'
            if (*path == '\0')
            {
                result.parent = root;
                result.name[0] = '\0';   // No final component
                return result;
            }
        }
        else
        {
            node = current;
        }

        char component[NAMESIZE];
        while (*path != '\0')
        {
            print("PATH: ");
            print(path);
            print("\n");
            
            // Skip repeated slashes
            while (*path == '/')
                path++;

            // Read one path component
            int i = 0;

            while (*path != '\0' && *path != '/')
            {
                if (i < NAMESIZE - 1)
                    component[i++] = *path;

                path++;
            }

            component[i] = '\0';

            /*print("COMPONENT: ");
            print(component);
            print("\n");*/
            if (streq(component, "."))
            {
                continue;
            }

            if (streq(component, ".."))
            {
                if (node->parent != nullptr)
                    node = node->parent;

                continue;
            }

            while (*path == '/')
                path++;
    
            if (*path == '\0') {
                /*print("Last component: '");
                print(component);
                print("'\n");*/

                result.parent = node;

                memcpy(result.name, component, NAMESIZE);
                result.name[NAMESIZE - 1] = '\0';

                /*print("Returning name: '");
                print(result.name);
                print("'\n");*/

                return result;
            }
            
            // Find this child inside the current node
            node = findNode(component, node, false);
            //print(intToString(err));
            if (node == nullptr) {
                /*print("The parent directory does not exist\n");*/
                err = true;
                return {};
            }
            
            if (node->type != Folder) {
                err = true;
                return {};
            }
        }
        /*print("COMPONENT: ");
        print(component);*/
        err = true;
        return {};
    }
    void ls(const char *n)
    {
        // print("this = ");
        // print(hexToString((uint32_t)this));
        FSNode* dir = resolvePath(n);
        if (dir->firstChild == nullptr)
        {
            print("(empty)\n");
            return;
        }
        FSNode *last = dir->firstChild;
        while (last)
        {
            print(last->name);
            if (last->type == Folder)
            {
                print("/");
            }

            if (dir->type == File)
            {
                print(dir->name);
                return;
            }
            print("\n");
            last = last->nextSibling;
        }
    }

    void cd(const char *n)
    {

        FSNode* dir = resolvePath(n);

        if (dir == nullptr) {
            print("<Err:>There's no such directory as: ");
            print(n);
            print("\n");
            return;
        }

        if (dir->type != Folder) {
            print(n);
            print(" is a file\n");
            return;
        }

        current = dir;
    }

    void pwd()
    {
        if (current->parent == nullptr)
        {
            print("/");
            return;
        }

        Stack<FSNode *, 64> dirs;
        FSNode *dir = current;
        while (dir->parent != nullptr)
        {
            dirs.push(dir);
            dir = dir->parent;
        }

        while (!dirs.empty())
        {
            FSNode *printme = dirs.pop();
            print("/");
            print(printme->name);
        }
        print("/");
        print("\n");
    }

    void create(const char *n)
    {
        bool err = false;
        ParentResult p = resolveParent(n, err);
        if (err) {
            print("Error when resolving the path");
        }
        if (p.parent == nullptr) {
            print(n);
            print(" does not exist\n");
            return;
        }


        FSNode *newFile = createNode(p.name, File, p.parent, 0);

        if (p.parent->firstChild == nullptr)
        {
            p.parent->firstChild = newFile;
            return;
        }
        else
        {
            if (findNode(p.name, p.parent, false))
            {
                print("File already exists.\n");
                return;
            }

            appendChild(p.parent, newFile);
        }
    }

    void gettype(const char *n)
    {
        FSNode* dir = resolvePath(n);
        if (dir == nullptr)
        {
            print("File not found\n");
            return;
        }

        switch (dir->type)
        {
        case File:
            print(dir->name);
            print(" is a file\n");
            break;
        default:
            print(dir->name);
            print(" is a folder\n");
            break;
        }
    }

    void write(const char *filename, const char *content, bool overwrite = true)
    {
        if (filename == nullptr || content == nullptr)
        {
            print("No valid arguments \n");
            return;
        }

        FSNode* file = resolvePath(filename);

        if (file == nullptr)
        {
            print(filename);
            print(" does not exist\n");
            return;
        }
        uint32_t len = 0;
        if (overwrite)
        {
            if (file->data != nullptr)
            {
                kfree(file->data);
            }
            len = strlen(content);
            file->data = (char *)kmalloc(len + 1);
            /*print("data ptr: ");
            print(hexToString((uint32_t)fileToModify->data));
            print("\n");

            print("content ptr: ");
            print(hexToString((uint32_t)content));
            print("\n");*/
            if (file->data == nullptr)
            {
                print("Out of memory\n");
            }
            strcpy(file->data, content);
            file->size = len;
            /*print("direct read: ");
            print(fileToModify->data);
            print("\n");*/
        }
        else
        {
            uint32_t oldLen = 0;

            if (file->data != nullptr)
            {
                oldLen = strlen(file->data);
            }

            uint32_t newLen = oldLen + strlen(content);

            char *temp = (char *)kmalloc(newLen + 1);

            if (temp == nullptr)
            {
                print("Out of memory\n");
                return;
            }

            temp[0] = '\0'; // important if old data is empty

            if (file->data != nullptr)
            {
                stradd(temp, file->data, content);
                kfree(file->data);
            }
            else
            {
                strcpy(temp, content);
            }

            file->data = temp;
            file->size = newLen;
        }
    }

    void read(const char *filename)
    {
        if (filename == nullptr)
        {
            print("No valid arguments \n");
            return;
        }

        FSNode* file = resolvePath(filename);
        if (file == nullptr)
        {
            print(filename);
            print(" does not exist\n");
            return;
        }

        if (file->type != File)
        {
            print(filename);
            print(" is not a file\n");
            return;
        }
        if (file == nullptr)
        {
            print(filename);
            print(" does not exist\n");
            return;
        }

        if (file->data == nullptr)
        {
            print("(Empty file)\n");
            return;
        }
        print(file->data);
        print("\n");
    }

    void size(const char *filename)
    {
        if (filename == nullptr)
        {
            print("Not a valid argument\n");
            return;
        }
        FSNode* file = resolvePath(filename);

        if (file == nullptr)
        {
            print(filename);
            print(" does not exist");
            return;
        }

        print("Current size of ");
        print(file->name);
        print(": ");
        print(intToString(file->size));
        print("\n");
        print("Unused bytes: ");
        print(intToString((4096 - 1) - file->size));
        print("\n");
    }

    void rm(const char *filename, bool recursive)
    {
        if (filename == nullptr)
        {
            print("Not a valid argument\n");
            return;
        }

        FSNode *node = resolvePath(filename);

        if (node == nullptr)
        {
            print("File ");
            print(filename);
            print(" does not exist\n");
            return;
        }
        if (recursive && node->type == Folder)
        {
            recursiveDestroy(node);
            return;
        }
        if (node->type != File)
        {
            print(filename);
            print(" is not a file\n");
            return;
        }
        destroyNode(node);
    }

    void rmdir(const char *dirname)
    {
        if (dirname == nullptr)
        {
            print("Not a valid agument\n");
            return;
        }

        FSNode *folderToRemove = resolvePath(dirname);
        if (folderToRemove == nullptr)
        {
            print("Folder ");
            print(dirname);
            print(" does not exist");
            return;
        }

        if (folderToRemove->type != Folder)
        {
            print(dirname);
            print(" is not a folder");
            return;
        }
        if (folderToRemove->firstChild != nullptr)
        {
            print(dirname);
            print(" is not empty!");
            return;
        }
        destroyNode(folderToRemove);
    }

    void recursiveDestroy(FSNode *folder)
    {

        FSNode *last = folder->firstChild;

        while (last)
        {
            FSNode *next = last->nextSibling;
            if (last->type == File)
            {
                destroyNode(last);
            }
            else if (last->type == Folder)
            {
                recursiveDestroy(last);
            }
            last = next;
        }
        destroyNode(folder);
    }

    void rename(const char *name, const char *destiny)
    {
        FSNode *node = resolvePath(name);
        if (node == nullptr)
        {
            print(name);
            print(" does not exist\n");
            return;
        }

        strcpy(node->name, destiny);
    }

    void copy(const char *name, const char *destiny)
    {
        FSNode *node = resolvePath(name);
        bool err = false;
        ParentResult dest = resolveParent(destiny, err);
        if (err) {
            print("Parent not found");
            return;
        }
        if (node == nullptr && destiny == nullptr)
        {
            print("No valid arguments\n");
            return;
        }
        if (node == nullptr)
        {
            print(name);
            print(" does not exist\n");
            return;
        }
        if (findNode(dest.name, dest.parent, false) != nullptr)
        {
            print(destiny);
            print(" already exists.\n");
            return;
        }

        FSNode *newCopy = cloneNode(node, dest.parent);

        strcpy(newCopy->name, dest.name);
        if (dest.parent->firstChild == nullptr)
        {
            dest.parent->firstChild = newCopy;
        }
        else
        {
           if (findNode(dest.name, dest.parent, false))
            {
                print("Destination already exists.\n");
                return;
            }

            appendChild(dest.parent, newCopy);
        }
    }

    void find(const char *name)
    {
        FSNode *node = resolvePath(name);
        if (node == nullptr)
        {
            print(name);
            print(" does not exist.");
            return;
        }

        Stack<FSNode *, 64> dirs;
        FSNode *dir = node;
        while (dir->parent != nullptr)
        {
            dirs.push(dir);
            dir = dir->parent;
        }

        print("File or Folder found at: ");
        while (!dirs.empty())
        {
            FSNode *printme = dirs.pop();
            print("/");
            print(printme->name);
        }
        if (dir->type == Folder)
        {
            print("/");
        }
        print("\n");
    }
    char *getData(FSNode *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        char *copiedData = nullptr;
        if (node->data != nullptr)
        {
            uint32_t len = strlen(node->data);
            copiedData = (char *)kmalloc(len + 1);
            strcpy(copiedData, node->data);
        }
        return copiedData;
    }
    FSNode *getCurrent()
    {
        return current;
    }
};

inline RamFS ramfs;