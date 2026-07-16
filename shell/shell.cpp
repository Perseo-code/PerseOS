#include "shell.hpp"


void shell_execute(const char* command) {
    const CuttedCommand splitCmd = cmd_cutter(command);
    const char* cmd = splitCmd.cmd;
    const char* args = splitCmd.args;
    for (int i = 0; i < CMDS; ++i) {
        if (streq(cmd, commands[i].name)) {
            commands[i].functionality(args);
            print("> ");
            return;
        }
    }
    print("Unknown Command:");
    print(cmd);
    print("\n");
    print("> ");
}


void shell_init() {
    setEnterCallback(shell_execute);
    print("> ");
}