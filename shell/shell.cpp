#include "shell.hpp"


void printShell() {
    FSNode* curDir = ramfs.getCurrent();
    print(curDir->name);
    print(" > ");
}

void shell_execute(const char* command) {
    ParsedCommand splitCmd = cmd_cutter(command);
    const char* cmd = splitCmd.cmd;
    const char* args = splitCmd.args;
    for (int i = 0; i < CMDS; ++i) {
        if (streq(cmd, commands[i].name)) {
            commands[i].functionality(splitCmd);
            input_length = 0;
            inputBuffer[0] = '\0';
            printShell();
            return;
        }
    }
    print("Unknown Command:");
    print(cmd);
    print("\n");
    printShell();
}

void shell_init() {
    setEnterCallback(shell_execute);
    printShell();
}