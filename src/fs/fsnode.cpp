#include <fs/fsnode.hpp>
#include <memory.hpp>
#include <string.hpp>

FSNode *createNode(const char *n, Types t, FSNode *p, uint32_t s, char *d, FSNode *f, FSNode *ns)
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

FSNode *findNode(const char *name, FSNode* current, FSNode *dir, bool recursive)
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

PFSNode* findPFSNode(const char* name, PFSNode* dir, ATA* disk, bool recursive) {
    if (dir == nullptr)
        return nullptr;
    
    
    loadPFSNode(dir, disk);

    if (dir->child == nullptr) return nullptr;
    PFSNode *node = dir->child;
    PFSNode *foundit;
    while (node)
    {
        if (node->type == Folder && recursive)
        {
            foundit = findPFSNode(name, node, disk, true);
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

void loadPFSNode(PFSNode* parent, ATA* disk) {
    if (parent == nullptr) return;
    if (parent->children_loaded) return;
    if (!parent->has_children) return;
    uint32_t lba = parent->child_sector;
    uint8_t* raw = (uint8_t*)kmalloc(SECTOR_SIZE);
    if (raw == nullptr) return;
    disk->read28(lba, raw);
    PFSNode* child = decodePFSNode(raw);
    if (child == nullptr) {
        parent->children_loaded = false;
        kfree(raw);
        return;
    }
    parent->child = child;
    while (child->sibling_sector != 0) {
        lba = child->sibling_sector;
        disk->read28(lba, raw);
        child->nextSibling = decodePFSNode(raw);
        if (child->nextSibling == nullptr) break;
        child = child->nextSibling;
    }
    parent->children_loaded = true;
    kfree(raw);
}

void freePFSNode(PFSNode* node) {
    if (node->parent == nullptr) {
        return;
    }
    if (node->extension == true) {
        kfree(node->extension_child);
    }
    PFSNode* parent = node->parent;
    if (parent->child == node) {
        parent->child = node->nextSibling;
        kfree(node);
        return;
    }
    PFSNode* prev = parent->child;
    while (prev && prev->nextSibling != node) {
        prev = prev->nextSibling;
    }

    if (prev)
        prev->nextSibling = node->nextSibling;
    
    kfree(node);
}

    bool appendPFSChild(PFSNode* parent, PFSNode* child, ATA* disk) {
        if (parent == nullptr || child == nullptr)
            return false;
        
        loadPFSNode(parent, disk);


        child->parent = parent;
        child->parent_sector = parent->sector;
        child->nextSibling = nullptr;
        child->sibling_sector = 0;
        if (parent->child == nullptr)
        {
            parent->has_children = true;
            parent->child = child;
            parent->child_sector = child->sector;
            uint8_t* raw_parent = encodePFSNode(parent);
            uint8_t* raw_child = encodePFSNode(child);
            disk->write28(parent->sector, raw_parent);
            disk->write28(child->sector, raw_child);
            kfree(raw_parent);
            kfree(raw_child);
            return true;
        }

        PFSNode* last = parent->child;

        while (last->nextSibling)
            last = last->nextSibling;

        last->sibling_sector = child->sector; 
        last->nextSibling = child;
        uint8_t* raw_last = encodePFSNode(last);
        uint8_t* raw_child = encodePFSNode(child);
        disk->write28(last->sector, raw_last);
        disk->write28(child->sector, raw_child);
        kfree(raw_last);
        kfree(raw_child);
        return true;
    }