#include <fs/pfs.hpp>
#include <memory.hpp>
using namespace FS;
// Create 4 bytes using unsigned integer (1 byte) array. Already sums the counter by 4.
#define C4BUUI(destiny, origin, index) \ 
    destiny = \
        ((uint32_t)origin[index] << 24) | \
        ((uint32_t)origin[index + 1] << 16) | \
        ((uint32_t)origin[index + 2] << 8) | \
        ((uint32_t)origin[index + 3]); \
    i += sizeof(uint32_t)
// Use 4 bytes in a single byte array
#define U4BISBA(r, i) *((uint32_t*)(r + i))
PFSNode* createPFSNode(Types type, const char* name, bool extension, PFSNode* child = nullptr) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    result->type = type;
    strcpy(result->name, name);
    result->name_size = strlen(name);
    result->file_size = 0;
    result->extension = extension;
    result->child = child;
    return result;
}

uint8_t* encodePFSNode(PFSNode* node) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    size_t length = node->name_size;
    int i = 0;
    for (; i < length; i++) {
        result[i] = (uint8_t)node->name[i];
    }

    result[i] = node->type << 1; 
    result[i] |= node->extension;
    i++;
    U4BISBA(result, i) = node->name_size;
    i += 4;
    U4BISBA(result, i) = node->file_size;
    i += 4;
    U4BISBA(result, i) = node->sector;
    i += 4;
    U4BISBA(result, i) = node->child_sector;
    return result;
}

PFSNode* decodePFSNode(const uint8_t* metadata) {
    PFSNode* result = (PFSNode*)kmalloc(sizeof(PFSNode));
    int i = 0;
    while (i < MAX_NAMESIZE) {
        result->name[i] = metadata[i];
        i++;
    }

    result->type = (Types)(metadata[i] >> 1);
    result->extension = (bool)(metadata[i] & 1);
    i++;
    C4BUUI(result->name_size, metadata, i);
    C4BUUI(result->file_size, metadata, i);
    C4BUUI(result->sector, metadata, i);
    C4BUUI(result->child_sector, metadata, i);
    return result;
}

uint8_t* encodeSuperblock(PFSSuperblock* block) {
    uint8_t* result = (uint8_t*)kmalloc(SECTOR_SIZE);
    int i = 0;
    U4BISBA(result, i) = block->magic;
    i += 4;
    U4BISBA(result, i) = block->version;
    i += 4;
    U4BISBA(result, i) = block->root_lba;
    i += 4;
    U4BISBA(result, i) = block->total_sectors;
    return result;
}

PFSSuperblock* decodeSuperblock(const uint8_t* metadata) {
    PFSSuperblock* result = (PFSSuperblock*)kmalloc(sizeof(PFSSuperblock));
    int i = 0;
    C4BUUI(result->magic, metadata, i);
    C4BUUI(result->version, metadata, i);
    C4BUUI(result->root_lba, metadata, i);
    C4BUUI(result->total_sectors, metadata, i);
    return result;
}

void PFS::format() {
    lba = 1;
    superblock = createSuperBlock(lba++, 2048);
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
}