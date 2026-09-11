#pragma once
#include <stdint.hpp>
#include <drivers/ATA/ATA.hpp>
#include <fs/fsnode.hpp>
namespace FS {

enum Type : bool {
    Directory,
    File
};

PFSNode* createPFSNode(Type, const char*, bool, PFSNode* parent = nullptr);
uint8_t* encodePFSNode(PFSNode* node);
class PFS {
private:
    ATA disk;
    PFSNode* root;
    PFSNode* current;
    uint32_t lba;
public:
    PFS(ATA disk) : disk(disk) { // Assuming the disk has been already initialized.
        root = createPFSNode(Directory, "/", false);
        current = root;
        lba = 1;
        disk.write28(lba, encodePFSNode(root));
    }
};
}