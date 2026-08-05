#pragma once
#include <io/io.hpp>
#include <error/error.hpp>
#include <stdint.hpp>

class ATA {
private:
    void cacheFlush() {
        outb(0x1F7, 0xE7);
    }

    void setLBAbits(uint32_t lba) {
        outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
        outb(0x1F2, 1);
        outb(0x1F3, lba & 0xFF);
        outb(0x1F4, (lba >> 8) & 0xFF);
        outb(0x1F5, (lba >> 16) & 0xFF);
    }
public:
    bool identify() {
        outb(0x1F6, 0xA0);
        outb(0x1F7, 0xEC);

        uint8_t status = inb(0x1F7);

        if (status == 0) {
            RAISE(DeviceErr, ERR_DRIVE_NOT_FOUND, false, "");
            return false;
        }
        return true;
    }

    void read28(uint32_t lba, uint8_t* buffer) {
        setLBAbits(lba);
        outb(0x1F7, 0x20); // READ SECTORS
        
        for (int i = 0; i < 256; i++) {
            ((uint16_t*)buffer)[i] = inw(0x1F0); // Data port
        }
    }

    void write28(uint32_t lba, const uint8_t* buffer) {
        setLBAbits(lba);
        outb(0x1F7, 0x30);
        for (int i = 0; i < 256; i++) {
            outw(0x1F0, ((uint16_t*)buffer)[i]);
        }
        cacheFlush(); // CACHE FLUSH
    }
};