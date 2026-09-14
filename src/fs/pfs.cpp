#include <fs/pfs.hpp>
#include <memory.hpp>
#include <fs/fsresolver.hpp>
#include <error/error.hpp>
using namespace FS;
#define WRITE_U32_LE(buf, idx, val) do { \
    (buf)[(idx) + 0] = (uint8_t)((uint32_t)(val) & 0xFF); \
    (buf)[(idx) + 1] = (uint8_t)(((uint32_t)(val) >> 8) & 0xFF); \
    (buf)[(idx) + 2] = (uint8_t)(((uint32_t)(val) >> 16) & 0xFF); \
    (buf)[(idx) + 3] = (uint8_t)(((uint32_t)(val) >> 24) & 0xFF); \
} while(0)

#define READ_U32_LE(buf, idx) ( \
    ((uint32_t)(buf)[(idx) + 0])       | \
    ((uint32_t)(buf)[(idx) + 1] << 8)  | \
    ((uint32_t)(buf)[(idx) + 2] << 16) | \
    ((uint32_t)(buf)[(idx) + 3] << 24)   \
)
PFSNode* createPFSNode(Types type, const char* name, bool extension, PFSNode* parent = nullptr) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    if (parent != nullptr) {
        result->parent = parent;
        parent->has_children = true;
        result->parent_sector = parent->sector;
    } else {
        result->parent = nullptr;
    }
    result->type = type;
    strcpy(result->name, name);
    result->name_size = strlen(name);
    result->file_size = 0;
    result->extension = extension;
    return result;
}

uint8_t* encodePFSNode(PFSNode* node) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    memset(result, 0, SECTOR_SIZE);
    size_t length = node->name_size;
    int i = 0;
    for (; i < length && i < MAX_NAMESIZE; i++) {
        result[i] = (uint8_t)node->name[i];
    }
    i = MAX_NAMESIZE;
    result[i] = (node->type & 1) << 2; 
    result[i] |= (node->extension & 1) << 1;
    result[i] |= (node->has_children & 1);
    i++;
    WRITE_U32_LE(result, i, node->name_size);
    i += 4;
    WRITE_U32_LE(result, i, node->file_size);
    i += 4;
    WRITE_U32_LE(result, i, node->sector);
    i += 4;
    WRITE_U32_LE(result, i, node->child_sector);
    i += 4;
    WRITE_U32_LE(result, i, node->parent_sector);
    i += 4;
    WRITE_U32_LE(result, i, node->sibling_sector);
    return result;
}

PFSNode* decodePFSNode(const uint8_t* metadata) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    int i = MAX_NAMESIZE;

    result->type = (Types)((metadata[i] >> 2) & 1);
    result->extension = (bool)((metadata[i] >> 1) & 1);
    result->has_children = (bool)(metadata[i] & 1);
    i++;
    result->name_size = READ_U32_LE(metadata, i);
    i += 4;
    result->file_size = READ_U32_LE(metadata, i);
    i += 4;
    result->sector = READ_U32_LE(metadata, i);
    i += 4;
    result->child_sector = READ_U32_LE(metadata, i);
    i += 4;
    result->parent_sector = READ_U32_LE(metadata, i);
    i += 4;
    result->sibling_sector = READ_U32_LE(metadata, i);

    i = 0;
    while (i < result->name_size) {
        result->name[i] = metadata[i];
        i++;
    }

    return result;
}

uint8_t* encodeSuperblock(PFSSuperblock* block) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    int i = 0;
    WRITE_U32_LE(result, i, block->magic);
    i += 4;
    WRITE_U32_LE(result, i, block->version);
    i += 4;
    WRITE_U32_LE(result, i, block->root_lba);
    i += 4;
    WRITE_U32_LE(result, i, block->total_sectors);
    return result;
}

PFSSuperblock* decodeSuperblock(const uint8_t* metadata) {
    PFSSuperblock* result = (PFSSuperblock*)kmalloc(sizeof(PFSSuperblock));
    int i = 0;
    result->magic = READ_U32_LE(metadata, i);
    i += 4;
    result->version = READ_U32_LE(metadata, i);
    i += 4;
    result->root_lba = READ_U32_LE(metadata, i);
    i += 4;
    result->total_sectors = READ_U32_LE(metadata, i);
    return result;
}

void PFS::format() {
    lba = 1;
    superblock = createSuperBlock(lba++, disk.getTotalSectors());
    root = createPFSNode(Folder, "/", false);
    current = root;
    uint8_t* buffer = encodeSuperblock(superblock);
    disk.write28(1, buffer);
    buffer = encodePFSNode(root);
    disk.write28(superblock->root_lba, buffer);
    lba++;
}

void PFS::mount() {
    lba = 1;
    uint8_t* buffer = (uint8_t*)kmalloc(SECTOR_SIZE);
    disk.read28(lba, buffer);
    superblock = decodeSuperblock(buffer);
    if (superblock->magic != PFS_MAGIC) {superblock->valid = false; return;}
    lba++;
    disk.read28(lba, buffer);
    root = decodePFSNode(buffer);
    current = root;
    loadPFSNode(root, &disk);
}

void PFS::create(const char* path) {
    PFSParentResult result = resolvePFSParent(path, root, current, &disk);
    if (result.err == true) {
        RAISE(FSError, ERR_RESOLVE, false, "Could not resolve the path to the parent");
        return;
    }
    if (result.parent == nullptr) {
        RAISE(FSError, ERR_NULLPTR, false, "The path does not exist");
        return;
    }

    PFSNode* newFile = createPFSNode(File, result.name, false);
    if (result.parent->child == nullptr)
    {
        result.parent->child = newFile;
    }
    else
    {
        if (findPFSNode(result.name, result.parent, &disk, false))
        {
            RAISE(FSError, ERR_ALREADY_EXISTS, false, "");
            return;
        }

        appendPFSChild(result.parent, newFile);
    }

    newFile->sector = lba;
    uint8_t* raw = encodePFSNode(newFile);
    disk.write28(lba, raw);
    lba++;
}