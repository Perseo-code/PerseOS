#pragma once
#include <stdint.hpp>
#include <drivers/ATA/ATA.hpp>
#include <fs/fsresolver.hpp>
#include <fs/fsnode.hpp>
#include <memory.hpp>
#define CURRENT_PFS_VERSION 0
namespace FS {

inline PFSSuperblock* createSuperBlock(uint32_t root_lba, uint32_t total_sectors) {
    PFSSuperblock* superblock = (PFSSuperblock*)kmalloc(sizeof(PFSSuperblock));
    if (root_lba == 1) {
        root_lba = 2;
    }
    superblock->magic = PFS_MAGIC;
    superblock->version = CURRENT_PFS_VERSION;
    superblock->root_lba = root_lba;
    superblock->total_sectors = total_sectors;
    return superblock;
}



class PFS {
private:
    ATA* disk;
    PFSSuperblock* superblock;
    PFSNode* root;
    PFSNode* current;
    uint32_t lba;
public:
    void init(ATA* disk) {
        this->disk = disk;
        lba = 0;
        superblock = nullptr;
        root = nullptr;
        current = nullptr;
    }

    bool disk_exists() {
        return disk->exists();
    }

    void format();
    void mount();
    void create(const char* path, Mode mode);
    PFSNode* open(const char* path, Mode mode);
    void mdir(const char* path, Mode mode);
    void ls(const char* path);
};
}

inline FS::PFS persistent_filesystem;