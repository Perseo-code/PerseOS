#pragma once
#include <drivers/vga/vga.hpp>
#define CMDS 2
typedef void (*func)(const char*);

struct CuttedCommand {
    char cmd[32];
    char args[224];
};

CuttedCommand cmd_cutter(const char* command);
struct CMD {
    func functionality;
    const char* name;
};

// Remember, the pointers AFTER the functions
void help(const char* = "");
inline void clear(const char* name = "") {
    clean_screen();
}

inline CMD commands[CMDS] = {
    { help, "help" },
    { clear, "clear"}
};