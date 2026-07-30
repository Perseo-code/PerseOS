#pragma once
#define ISAWARNING(var) \
    print(var ? "<WARNING:>" : "<ERR:>")
#define SCASE(ErrType, msg) \
    case ErrType: print_err(msg); break
typedef enum {
    GENERIC_OK = 0,
    // Generic
    ERR_UNKNOWN,
    ERR_INVALID_ARGUMENT,
    ERR_MISSING_ARGUMENT,
    ERR_TOO_MANY_ARGUMENTS,
    ERR_INVALID_PATH,
    ERR_INVALID_NAME,
    ERR_OUT_OF_MEMORY,
    ERR_NOT_IMPLEMENTED,
    ERR_PERMISSION_DENIED,
    ERR_INTERNAL
} ErrGeneric;

typedef enum {
    /* Filesystem */
    ERR_NOT_FOUND,
    ERR_ALREADY_EXISTS,
    ERR_PARENT_NOT_FOUND,

    /* Node type */
    ERR_NOT_A_FILE,
    ERR_NOT_A_FOLDER,
    ERR_IS_FILE,
    ERR_IS_FOLDER,

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
} FSErr;
typedef enum
{
    SHELL_OK = 0,

    /* Shell */
    ERR_UNKNOWN_COMMAND,
    ERR_INVALID_SYNTAX,
    ERR_MISSING_QUOTE

} ShellErr;

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
} DeviceErr;

class Error {
public:
    Error();
    ~Error();
    virtual void raise() = 0;
};

class ShellError : Error {
    ShellErr err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg.join(" "));
        print(args);
        print("\n");
    }
public:
    ShellError(ShellErr se, bool w, string cmdname) : err(se), warning(w), args(cmdname) {}
    ~ShellError();
    void raise() override {
        switch (err) {
            ISAWARNING(warning);
            SCASE(ERR_UNKNOWN_COMMAND, "Unknown Command");
            SCASE(ERR_INVALID_SYNTAX, "Invalid Syntax");
            SCASE(ERR_MISSING_QUOTE, "Missing Quote");
            default:
                print("Error type not specified\n");
                break;
        }
    }
};

class FSError : Error {
    FSErr err;
    bool warning;
    string args;
    void print_err(string msg) {
        print(msg.join(" :"));
        print(args);
        print("\n");
    }
public:
    FSError(FSErr e, bool w, string a) : err(e), warning(w), args(a) {}
    void raise() override {
        ISAWARNING(warning);
        switch (err) {
            SCASE(ERR_NOT_FOUND, "File/Folder not found");
            SCASE(ERR_ALREADY_EXISTS, "File/Folder already exists");
            default:
                print("No error specified.\n");
                break;
        }
    }
}; 