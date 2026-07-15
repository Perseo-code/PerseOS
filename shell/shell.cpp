#include "shell.hpp"


void shell_execute(const char* command) {
    const CuttedCommand splitCmd = cmd_cutter(command);
    const char* cmd = splitCmd.cmd;
    for (int i = 0; i < CMDS; ++i) {
        if (streq(command, commands[i].name)) {
            commands[i].functionality("");
        }
    }
    print("> ");
}

void shell_init() {
    setEnterCallback(shell_execute);
    print("> ");
}