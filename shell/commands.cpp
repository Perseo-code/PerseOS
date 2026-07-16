#include "commands.hpp"

CuttedCommand cmd_cutter(const char* command){
    CuttedCommand result = {0};

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
        print(num);
        print("\n");
    }
}

void echo(const char* whattoprint) {
    print(whattoprint);
    print("\n");
}