#pragma once
#include <drivers/vga/vga.hpp>
#include <stddef.hpp>
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
void echo(const char*);

inline void version(const char* n) {
    print("PerseOS v0.1");
    print("\n");
}
inline void clear(const char* name = "") {
    clean_screen();
}


inline CMD commands[] = {
    { help, "help" },
    { version, "version" },
    { clear, "clear"},
    { echo, "echo" }
};

constexpr size_t CMDS =
    sizeof(commands) / sizeof(commands[0]);