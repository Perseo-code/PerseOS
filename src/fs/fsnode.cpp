#include <fs/fsnode.hpp>
#include <memory.hpp>
#include <string.hpp>
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

FSNode *findNode(const char *name, FSNode* current, FSNode *dir = nullptr, bool recursive = false)
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
            foundit = findNode(name, current, node, true);
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

PFSNode* findPFSNode(const char* name, PFSNode* dir, bool recursive = false) {
    if (dir == nullptr)
        return nullptr;
    if (dir->child == nullptr)
    {
        return nullptr;
    }

    PFSNode *node = dir->child;
    PFSNode *foundit;
    while (node)
    {
        if (node->type == Folder && recursive)
        {
            foundit = findPFSNode(name, node, true);
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