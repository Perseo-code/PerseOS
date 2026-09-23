#pragma once
#include <io/io.hpp>
#include <error/error.hpp>
#include <stdint.hpp>
#include <timer/timer.hpp>
#define DEFAULT_ATA_TIMEOUT 1000
#define DEFAULT_ATA_RWAIT_TIME 5
enum ATAResult {
    ATA_OK,
    ATA_TIMEOUT,
    ATA_ERR,
    ATA_DEV_FAULT // Yes, it's my fault. Get it?
};


constexpr uint32_t SECTOR_SIZE = 512;

// Every port for ATA
constexpr uint16_t ATA_DATA_PORT = 0x1F0;
constexpr uint16_t ATA_ERRFEAT_PORT = 0x1F1;
constexpr uint16_t ATA_SECTOR_COUNT = 0x1F2;
constexpr uint16_t ATA_LBA_LOW = 0x1F3;
constexpr uint16_t ATA_LBA_MID = 0x1F4;
constexpr uint16_t ATA_LBA_HIGH = 0x1F5;
constexpr uint16_t ATA_DRIVE_HEAD_SELECT = 0x1F6;
constexpr uint16_t ATA_CMD_PORT = 0x1F7;

// Commands
constexpr uint8_t  ATA_CMD_READ        = 0x20;
constexpr uint8_t  ATA_CMD_WRITE       = 0x30;
constexpr uint8_t  ATA_CMD_IDENTIFY    = 0xEC;
constexpr uint8_t  ATA_CMD_CACHE_FLUSH = 0xE7;

// Status
constexpr uint8_t ATA_SR_ERR = 0x01;
constexpr uint8_t ATA_SR_DRQ = 0x08;
constexpr uint8_t ATA_SR_DF = 0x20;
constexpr uint8_t ATA_SR_BSY = 0x80;

class ATA {
private:
    uint16_t identity_buf[256];
    bool device_found = false;
    void cacheFlush() {
        outb(ATA_CMD_PORT, ATA_CMD_CACHE_FLUSH);
    }

    void setLBAbits(uint32_t lba) {
        outb(ATA_DRIVE_HEAD_SELECT, (uint8_t)(0xE0 | ((lba >> 24) & 0x0F)));
        outb(ATA_SECTOR_COUNT, 1);
        outb(ATA_LBA_LOW, (uint8_t)(lba & 0xFF));
        outb(ATA_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
        outb(ATA_LBA_HIGH, (uint8_t)((lba >> 16) & 0xFF));
    }

    void ataWait() { 
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
        inb(ATA_CMD_PORT);
    }   

    ATAResult waitForDRQ() {
        print("Executing waitForDRQ\n");
        ataWait();
        print("finished ATA wait");
        Time::resetTimer();
        // We only break out when BUSY clears AND DATA REQUEST becomes active.
        while (Time::getTimer() < DEFAULT_ATA_TIMEOUT) {
            print("Timer has fired = ");
            print(intToString(irq0Count));
            print(" times\n");  
            uint8_t status = inb(ATA_CMD_PORT);
            print("Started loop\n");
            if (status & ATA_SR_DF) {
                print("Device fault\n");
                return ATA_DEV_FAULT;
            }

            if (status & ATA_SR_ERR) {
                print("Device Error\n");
                return ATA_ERR;
            }

            if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) {
                print("Works okay\n");
                return ATA_OK;
            }
        }
        print("Timed out \n");
        return ATA_TIMEOUT; // Timeout!
    }
public:
    bool init() {
        outb(ATA_DRIVE_HEAD_SELECT, 0xA0);
        outb(ATA_SECTOR_COUNT, 0);
        outb(ATA_LBA_LOW, 0);
        outb(ATA_LBA_MID, 0);
        outb(ATA_LBA_HIGH, 0);
        
        outb(ATA_CMD_PORT, ATA_CMD_IDENTIFY);
        ataWait(); 

        uint8_t status = inb(ATA_CMD_PORT);
        if (status == 0) {
            device_found = false;
            RAISE(DeviceError, ERR_DEVICE_NOT_FOUND, false, "Couldn't find the ATA disk");
            return false; 
        }

        // Wait for drive to finish processing
        Time::resetTimer();

        while (Time::getTimer() < DEFAULT_ATA_TIMEOUT) {
            status = inb(ATA_CMD_PORT);
            if (!(status & ATA_SR_BSY)) {
                break;
            }
        }

        // Check if data is actually ready
        if (status & ATA_SR_BSY) {
            device_found = false;
            RAISE(DeviceError, ERR_TIMEOUT, false, "ATA Identify timed out");
            return false;
        }

        if (status & ATA_SR_DF) {
            device_found = false;
            RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false, "ATA Device fault");
            return false;
        }

        if (status & ATA_SR_ERR) {
            device_found = false;
            RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false, "Could not prepare ATA device");
            return false;
        }

        // DRQ must be set before reading the 512-byte IDENTIFY data
        if (!(status & ATA_SR_DRQ))
        {
            device_found = false;
            RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false,
                "ATA IDENTIFY returned no data");
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
        if (lba > 0x0FFFFFFF) {
            RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false,
                "LBA exceeds ATA LBA28 range");
            return;
        }

        if (!device_found) {
            RAISE(DeviceError, ERR_DEVICE_NOT_FOUND, false, "ATA Device Not Found");    
            return;
        }
        setLBAbits(lba);
        outb(ATA_CMD_PORT, ATA_CMD_READ); // READ SECTOR
        ATAResult wdrq = waitForDRQ();
        if (wdrq != ATA_OK) {
            //uint8_t err_reg = inb(ATA_ERRFEAT_PORT); // Read why it failed
            if (wdrq == ATA_ERR) RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false, "Failed to read");
            else if (wdrq == ATA_DEV_FAULT) RAISE(DeviceError, ERR_DEVICE_READ_FAILED, false, "Device Faulted");
            else RAISE(DeviceError, ERR_TIMEOUT, false, "Device Timed out");
            return;
        }
        uint16_t* ptr = (uint16_t*)buffer;
        for (int i = 0; i < 256; i++) {
            ptr[i] = inw(ATA_DATA_PORT); // Data port
        }
    }

    void write28(uint32_t lba, const uint8_t* buffer) {
        print("Write process requested\n");
        if (lba > 0x0FFFFFFF) {
            RAISE(DeviceError, ERR_DEVICE_WRITE_FAILED, false,
                "LBA exceeds ATA LBA28 range");
            return;
        }
        print("Checking wether the device exists...");
        if (!device_found) {
            RAISE(DeviceError, ERR_DEVICE_NOT_FOUND, false, "ATA Device Not found");
            return;
        }
        print(" Done\n");
        print("Setting up lba...");
        setLBAbits(lba);
        print(" Done\n");
        print("Requesting the write using the port...");
        outb(ATA_CMD_PORT, ATA_CMD_WRITE);
        print(" Done\n");
        ATAResult n = waitForDRQ();
        print("After waitfordrq\n");
        if (n != ATA_OK) {
            // uint8_t err_reg = inb(ATA_ERRFEAT_PORT); // Read why it failed
            if (n == ATA_ERR)
                RAISE(DeviceError, ERR_DEVICE_WRITE_FAILED, false,
                    "Device Write Failed");
            else if (n == ATA_DEV_FAULT)
                RAISE(DeviceError, ERR_DEVICE_WRITE_FAILED, false,
                    "ATA Device Fault");
            else if (n == ATA_TIMEOUT)
                RAISE(DeviceError, ERR_TIMEOUT, false,
                    "ATA Drive Timed Out");
            return;
        }
        print("Starting to write.\n");
        const uint16_t* ptr = (uint16_t*)buffer;
        for (int i = 0; i < 256; i++) {
            print("Writing word ");
            print(intToString(i));
            print("...");
            outw(ATA_DATA_PORT, ptr[i]);
            print(" Done\n");
        }
        cacheFlush(); // CACHE FLUSH
    }
};