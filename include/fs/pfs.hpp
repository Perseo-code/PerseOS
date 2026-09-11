#pragma once
#include <stdint.hpp>
#include <drivers/ATA/ATA.hpp>
#include <fs/fsnode.hpp>
namespace FS {


PFSNode* createPFSNode(Types, const char*, bool, PFSNode* child = nullptr);
uint8_t* encodePFSNode(PFSNode* node);
PFSNode* decodePFSNode(const uint8_t* metadata);
class PFS {
private:
    ATA disk;
    PFSNode* root;
    PFSNode* current;
    uint32_t lba;
public:
    PFS(ATA disk) : disk(disk) { // Assuming the disk has been already initialized.
        root = createPFSNode(Folder, "/", false);
        current = root;
        lba = 1;
        disk.write28(lba, encodePFSNode(root));
    }
};
}