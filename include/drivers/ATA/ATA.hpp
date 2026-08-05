#pragma once
#include <io/io.hpp>
#include <error/error.hpp>
#include <stdint.hpp>
// Every port for ATA
const uint16_t ATA_DATA_PORT = 0x1F0;
const uint16_t ATA_ERRFEAT_PORT = 0x1F1;
const uint16_t ATA_SECTOR_COUNT = 0x1F2;
const uint16_t ATA_LBA_LOW = 0x1F3;
const uint16_t ATA_LBA_MID = 0x1F4;
const uint16_t ATA_LBA_HIGH = 0x1F5;
const uint16_t ATA_DRIVE_HEAD_SELECT = 0x1F6;
const uint16_t ATA_CMD_PORT = 0x1F7;

// Commands
constexpr uint8_t  ATA_CMD_READ        = 0x20;
constexpr uint8_t  ATA_CMD_WRITE       = 0x30;
constexpr uint8_t  ATA_CMD_IDENTIFY    = 0xEC;
constexpr uint8_t  ATA_CMD_CACHE_FLUSH = 0xE7;

class ATA {
private:
    uint16_t identity_buf[256];
    bool device_found = false;
    void cacheFlush() {
        outb(ATA_CMD_PORT, ATA_CMD_CACHE_FLUSH);
    }

    void setLBAbits(uint32_t lba) {
        outb(ATA_DRIVE_HEAD_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
        outb(ATA_SECTOR_COUNT, 1);
        outb(ATA_LBA_LOW, lba & 0xFF);
        outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
        outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    }

    void reactWait() { // 400ns delay
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
    }

    bool waitForReady() {
        reactWait(); // Always wait 400ns first for status to update
        
        // Loop while Busy (0x80) is set, OR while Data Request (0x08) is clear.
        // We only break out when BUSY clears AND DATA REQUEST becomes active.
        while (true) {
            uint8_t status = inb(ATA_CMD_PORT);
            if (status & 0x01) {
                return false; 
            }
        
            if (status & 0x20) {
                return false;
            }

            if (!(status & 0x80) && (status & 0x08)) {
                return true;
            }
        }
    }
public:
    bool init() {
        outb(ATA_DRIVE_HEAD_SELECT, 0xA0); 
        outb(ATA_SECTOR_COUNT, 0);
        outb(ATA_LBA_LOW, 0);
        outb(ATA_LBA_MID, 0);
        outb(ATA_LBA_HIGH, 0);
        
        outb(ATA_CMD_PORT, ATA_CMD_IDENTIFY);
        reactWait(); 

        uint8_t status = inb(ATA_CMD_PORT);
        if (status == 0) {
            device_found = false;
            return false; 
        }

        // Wait for drive to finish processing
        while (status & 0x80) {
            status = inb(ATA_CMD_PORT);
        }

        // Check if data is actually ready
        if (!(status & 0x08)) {
            device_found = false;
            return false;
        }

        // Read and CACHE the 512 bytes of drive info safely into the class
        for (int i = 0; i < 256; i++) {
            identity_buf[i] = inw(ATA_DATA_PORT);
        }

        device_found = true;
        return true;
    }

    bool exists() {
        return device_found;
    }
    uint32_t getTotalSectors() const {
        if (!device_found) return 0;
        uint32_t sectors = identity_buf[60] | ((uint32_t)(identity_buf[61]) << 16); // The information can be found here
        return sectors;
    }
    void read28(uint32_t lba, uint8_t* buffer) {
        if (!device_found) {
            RAISE(DeviceError, ERR_DEVICE_NOT_FOUND, false, "");    
            return;
        }
        setLBAbits(lba);
        outb(ATA_CMD_PORT, ATA_CMD_READ); // READ SECTOR
        if (!waitForReady()) {
            uint8_t err_reg = inb(ATA_ERRFEAT_PORT); // Read why it failed
            RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false, "ATA Read failed");
            return;
        }
        uint16_t* ptr = (uint16_t*)buffer;
        for (int i = 0; i < 256; i++) {
            ptr[i] = inw(ATA_DATA_PORT); // Data port
        }
    }

    void write28(uint32_t lba, const uint8_t* buffer) {
        if (!device_found) {
            RAISE(DeviceError, ERR_DEVICE_NOT_FOUND, false, "");
            return;
        }
        setLBAbits(lba);
        outb(ATA_CMD_PORT, ATA_CMD_WRITE);
        if (!waitForReady()) {
            uint8_t err_reg = inb(ATA_ERRFEAT_PORT); // Read why it failed
            RAISE(DeviceError, ERR_DEVICE_WRITE_FAILED, false, "ATA Write failed");
            return;
        }
        const uint16_t* ptr = (uint16_t*) buffer;
        for (int i = 0; i < 256; i++) {
            outw(ATA_DATA_PORT, ptr[i]);
        }
        cacheFlush(); // CACHE FLUSH
    }
};