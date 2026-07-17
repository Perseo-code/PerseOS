#pragma once
#include <drivers/vga/vga.hpp>
#include <stddef.hpp>
#include <fs/ramfs.hpp>

typedef void (*func)(const char*);

struct CuttedCommand {
    char cmd[32];
    char args[224];
};

CuttedCommand cmd_cutter(const char* command);
struct CMD {
    func functionality;
    const char* name;
    const char* description;
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

inline void list(const char* n) {
    ramfs.ls(n);
}

inline void makedir(const char* n) {
    ramfs.mkdir(n);
}

inline void changedir(const char* n) {
    ramfs.cd(n);
}

inline void yourpath(const char*) {
    ramfs.pwd();
}
inline CMD commands[] = {
    { help, "help", "Show this message"},
    { version, "version", "See the OS's version" },
    { clear, "clear", "Clear the screen"},
    { echo, "echo", "Write something on the terminal. Use: echo <message>" },
    { list, "ls", "Print the files that exist in this folder" },
    { makedir, "mkdir", "Create a folder with <arg> name"},
    { changedir, "cd", "Move through the filesystem" },
    { yourpath, "pwd", "Print your location in the filesystem" }
};

constexpr size_t CMDS =
    sizeof(commands) / sizeof(commands[0]);