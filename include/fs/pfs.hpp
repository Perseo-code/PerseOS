#pragma once
#include <stdint.hpp>
#include <drivers/ATA/ATA.hpp>
#include <fs/fsnode.hpp>
#define CURRENT_PFS_VERSION 0
namespace FS {

PFSSuperblock* createSuperBlock(uint32_t root_lba, uint32_t total_sectors) {
    PFSSuperblock* superblock = (PFSSuperblock*)kmalloc(sizeof(PFSSuperblock));
    superblock->magic = PFS_MAGIC;
    superblock->version = CURRENT_PFS_VERSION;
    superblock->root_lba = root_lba;
    superblock->total_sectors = total_sectors;
    return superblock;
}

enum Mode {
    ReadOnly,
    ReadWrite,
    Append
};

class PFS {
private:
    ATA disk;
    PFSSuperblock* superblock;
    PFSNode* root;
    PFSNode* current;
    uint32_t lba;
public:
    PFS(ATA disk) : disk(disk) {
        lba = 0;
        superblock = nullptr;
        root = nullptr;
        current = nullptr;
    }
    void format();
    void mount();
    void create(const char* path);
    void open(const char* path, Mode mode);
};
}