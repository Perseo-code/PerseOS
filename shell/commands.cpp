#include "commands.hpp"

void ParsedCommand::tokenize() {
    argc = 0;
    int i = 0;
    while (args[i] != '\0') {
        while (args[i] == ' ') i++;
        if (args[i] == '\0') return;
        int j = 0;
        while ((args[i] != ' ') && (args[i] != '\0'))
        {
            if (j >= MAX_ARG_LENGTH - 1) {
                while (args[i] != ' ' && args[i] != '\0') {
                    i++;
                }
            }
            argv[argc][j++] = args[i++];
        }
        
        argv[argc][j] = '\0';
        argc++;
    }
}

ParsedCommand cmd_cutter(const char* command){
    ParsedCommand result = {0};

    uint32_t i = 0;

    // Copy command name
    while (command[i] && command[i] != ' ')
    {
        result.cmd[i] = command[i];
        i++;
    }
    result.cmd[i] = '\0';

    // Skip spaces
    while (command[i] == ' ')
        i++;

    // Copy arguments
    uint32_t j = 0;
    while (command[i])
        result.args[j++] = command[i++];
    result.args[j] = '\0';
    return result;
}

void help(const char* name) {
    print("Commands: \n");
    for (int i = 0; i < CMDS; i++) {
        string num = "CMD ";
        num.join(intToString(i));
        num.join(": ");
        num.join(commands[i].name);
        num.join(" -> ");
        num.join(commands[i].description);
        print(num);
        print("\n");
    }
}

void echo(const char* whattoprint) {
    print(whattoprint);
    print("\n");
}