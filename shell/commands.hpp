#pragma once
#include <drivers/vga/vga.hpp>
#include <stddef.hpp>
#include <fs/ramfs.hpp>
#include "shell_error/error.hpp"

#define MAX_ARGS 15
#define MAX_ARG_LENGTH 64
#define MAX_ARGS_LENGTH 224
#define MAX_CMD_LENGTH 32
struct ParsedCommand {
    char cmd[MAX_CMD_LENGTH];
    char args[MAX_ARGS_LENGTH];
    char argv[MAX_ARGS][MAX_ARG_LENGTH];
    int argc;
    Err err;
    void tokenize();
};

typedef void (*func)(ParsedCommand&);
ParsedCommand cmd_cutter(const char* command);
struct CMD {
    func functionality;
    const char* name;
    const char* description;
};

// Remember, the pointers AFTER the functions
void help(ParsedCommand&);
void echo(ParsedCommand&);

inline void version(ParsedCommand&) {
    print("PerseOS v0.2");
    print("\n");
}
inline void clear(ParsedCommand&) {
    clean_screen();
}

inline void list(ParsedCommand& n) {
    ramfs.ls(n.argv[0]);
}

inline void makedir(ParsedCommand& n) {
    ramfs.mkdir(n.argv[0]);
}

inline void changedir(ParsedCommand& n) {
    ramfs.cd(n.argv[0]);
}

inline void yourpath(ParsedCommand&) {
    ramfs.pwd();
}

inline void createFile(ParsedCommand& n) {
    ramfs.create(n.argv[0]);
}

inline void type(ParsedCommand& n) {
    ramfs.gettype(n.argv[0]);
}

inline void write(ParsedCommand& n) {
    bool _override = false;
    if (streq(n.argv[2], "true")) _override = true;
    ramfs.write(n.argv[0], n.argv[1], _override);
}

inline void read(ParsedCommand& n) {
    ramfs.read(n.argv[0]);
}

inline void size(ParsedCommand& n) {
    ramfs.size(n.argv[0]);
}

inline void remove(ParsedCommand& n) {
    bool recursive = streq(n.argv[1], "r");
    ramfs.rm(n.argv[0], recursive);
}

inline void removeDir(ParsedCommand& n) {
    ramfs.rmdir(n.argv[0]);
}
inline CMD commands[] = {
    { help, "help", "Show this message"},
    { version, "version", "See the OS's version" },
    { clear, "clear", "Clear the screen"},
    { echo, "echo", "Write something on the terminal. Use: echo <message>" },
    { list, "ls", "Print the files that exist in this folder" },
    { makedir, "mkdir", "Create a folder with <arg> name"},
    { changedir, "cd", "Move through the filesystem" },
    { yourpath, "pwd", "Print your location in the filesystem" },
    { createFile, "touch", "Create a file" },
    { write, "write", "Write somthing onto a file. \nArgs: write <filename> <text content> <override (true or nothing)>"},
    { read, "cat", "Read a file. Args: cat <filename>"},
    { remove, "rm", "Remove a file. Args: rm <filename/dirname> <r (to remove recursively a folder)>"},
    { removeDir, "rmdir", "Remove a directory. (Doesn't work if it's not empty)"},
    { size, "size", "Print the size of a file. Args: size <filename>"},
    { type, "type", "Print if <arg> is a folder or a file" }
};

constexpr size_t CMDS =
    sizeof(commands) / sizeof(commands[0]);