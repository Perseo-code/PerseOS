#pragma once
#include <drivers/vga/vga.hpp>
#define ISAWARNING(var) \
    print(var ? "<WARNING:>" : "<ERR:>")
#define RAISE(type, specific, warning, str) \
    type(specific, warning, str).raise()

typedef enum {
    ERR_INVALID_ARGS,
    ERR_OUT_OF_MEMORY,
    GENERIC_END
} ErrGeneric;

inline const char* GenericMsg[GENERIC_END] = {
    "Invalid Args",
    "Out of Memory"
};

typedef enum {
    /* Filesystem */
    ERR_NOT_FOUND,
    ERR_ALREADY_EXISTS,
    ERR_PARENT_NOT_FOUND,

    /* Node type */
    ERR_NOT_A_FILE,
    ERR_NOT_A_FOLDER,

    /* Directory */
    ERR_DIRECTORY_NOT_EMPTY,
    ERR_DIRECTORY_BUSY,
    ERR_CANNOT_REMOVE_ROOT,
    ERR_CANNOT_RENAME_ROOT,

    /* File */
    ERR_FILE_EMPTY,
    ERR_READ_FAILED,
    ERR_WRITE_FAILED,

    /* Copy / Move */
    ERR_SOURCE_NOT_FOUND,
    ERR_DESTINATION_EXISTS,
    ERR_SAME_SOURCE_DESTINATION,
    ERR_COPY_INTO_SELF,
    ERR_MOVE_INTO_SELF,
    // Resolve Err
    ERR_RESOLVE,
    ERR_NULLPTR,
    FS_END
} FSErr;

inline const char* FSMsg[FS_END] = {
    "Not Found",
    "Already Exists",
    "Parent not found",
    "Not a file",
    "Not a folder",
    "Directory not empty",
    "Directory Busy",
    "Cannot Remove Root",
    "Cannot Rename Root",
    "File Empty",
    "Read failed",
    "Write failed",
    "Source not found",
    "Destination Exists",
    "Same source destination",
    "Copy into self",
    "Move into self",
    "Error when resolving the path",
    "Does not exist"
};

typedef enum
{
    /* Shell */
    ERR_UNKNOWN_COMMAND,
    ERR_INVALID_SYNTAX,
    ERR_MISSING_QUOTE,
    SHELL_END
} ShellErr;

inline const char* const ShellMsg[SHELL_END] {
    "Unknown Command",
    "Invalid Syntax",
    "Missing Quote"
};

typedef enum {
    /* Devices */
    ERR_DEVICE_NOT_FOUND,
    ERR_DEVICE_BUSY,
    ERR_DEVICE_READ_FAILED,
    ERR_DEVICE_WRITE_FAILED,

    /* ATA */
    ERR_DRIVE_NOT_FOUND,
    ERR_SECTOR_OUT_OF_RANGE,
    ERR_DISK_FULL,
    ERR_TIMEOUT,
    DEVICE_END
} DeviceErr;

inline const char* DeviceMsg[DEVICE_END] {
    "Device Not Found",
    "Device Busy",
    "Device Read Failed",
    "Device Write Failed",
    // ATA (For the future)
    "ATA Drive not found",
    "Sector out of range",
    "Full disk",
    "Timeout"
};

class Error {
public:
    virtual void raise() = 0;
};


class GenericError : Error {
    ErrGeneric err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg);
        if (args.empty()) {
            print("\n");
            return;
        }
        print(" : ");
        print(args);
        print("\n");
    }
public:
    GenericError(ErrGeneric eg, bool w, string a = "") : err(eg), warning(w), args(a) {}
    void raise() override {
        ISAWARNING(warning);
        print_err(GenericMsg[err]);
    }
};

class ShellError : Error {
    ShellErr err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg);
        if (args.empty()) {
            print("\n");
            return;
        }
        print(" : ");
        print(args);
        print("\n");
    }
public:
    ShellError(ShellErr se, bool w, string cmdname = "") : err(se), warning(w), args(cmdname) {}
    void raise() override {
        ISAWARNING(warning);
        print_err(ShellMsg[err]);
    }
};

class FSError : Error {
    FSErr err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg);
        if (args.empty()) {
            print("\n");
            return;
        }
        print(" : ");
        print(args);
        print("\n");
    }
public:
    FSError(FSErr e, bool w, string a = "") : err(e), warning(w), args(a) {}
    void raise() override {
        ISAWARNING(warning);
        print_err(FSMsg[err]);        
    }
}; 

class DeviceError : Error {
    DeviceErr err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg);
        if (args.empty()) {
            print("\n");
            return;
        }
        print(" : ");
        print(args);
        print("\n");
    }
public:
    DeviceError(DeviceErr e, bool w, string a = "") : err(e), warning(w), args(a) {}
    void raise() override {
        ISAWARNING(warning);
        print_err(DeviceMsg[err]);
    }
};
