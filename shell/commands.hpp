#pragma once
#include <drivers/vga/vga.hpp>
#include <stddef.hpp>
#include <fs/ramfs.hpp>


#define MAX_ARGS 15
#define MAX_ARG_LENGTH 64
#define MAX_ARGS_LENGTH 224
#define MAX_CMD_LENGTH 32
struct ParsedCommand {
    char cmd[MAX_CMD_LENGTH];
    char args[MAX_ARGS_LENGTH];
    char argv[MAX_ARGS][MAX_ARG_LENGTH];
    int argc;
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
    ramfs.ls(n.argv[1]);
}

inline void makedir(ParsedCommand& n) {
    ramfs.mkdir(n.argv[1]);
}

inline void changedir(ParsedCommand& n) {
    ramfs.cd(n.argv[1]);
}

inline void yourpath(ParsedCommand&) {
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