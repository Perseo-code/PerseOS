#include "commands.hpp"

void ParsedCommand::tokenize() {
    argc = 0;
    int i = 0;
    bool inQuotes = false;
    while (args[i] != '\0') {
        while (args[i] == ' ') i++;
        if (args[i] == '\0') return;

        int j = 0;
        while ((args[i] != '\0'))
        {
            if (args[i] == '"') {
                inQuotes = !inQuotes;
                i++;

                if (inQuotes) {
                    continue;
                }
                else {
                    break;
                }
            }
            if (args[i] == ' ' && !inQuotes) {
                break;
            }
            if (j >= MAX_ARG_LENGTH - 1) {
                while (args[i] != '\0') {
                    if (args[i] == ' ' && !inQuotes) {
                        break;
                    }
                    else if (args[i] == '"' && inQuotes) {
                        inQuotes = false;
                        i++;
                        break;
                    }
                    i++;
                }
            } else {
                argv[argc][j++] = args[i++];
            }
        }
        
        if (inQuotes) {
            RAISE(ShellError, ERR_MISSING_QUOTE, true, nullptr);
            inQuotes = false;
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
    result.tokenize();
    return result;
}

void help(ParsedCommand& n) {  
    print("Commands: \n");
    for (int i = 0; i < CMDS; i++) {
        string num = commands[i].name;
        num.join(" -> ");
        num.join(commands[i].description);
        print(num);
        print("\n");
    }
}

void man(ParsedCommand& name) {
    bool found = false;
    int i = 0; 
    for (;i < CMDS; i++) {
        if (streq(commands[i].name, name.argv[0])) {
            found = true;
            break;
        }
    }

    if (!found) {
        print("The command ");
        print(name.argv[0]);
        print(" does not exist");
        return;
    }

    string num = commands[i].name;
    num.join(" -> ");
    num.join(commands[i].description);
    print(num);
    print("\n");
    return;
}

void echo(ParsedCommand& whattoprint) {
    print(whattoprint.args);
    print("\n");
}

void mem(ParsedCommand& n) {
    BlockHeader* last = firstBlock;
    BlockHeader* before;
    uint32_t usedBlocks = 0;
    uint32_t freeBlocks = 0;
    uint32_t blocks = 0;
    uint32_t usedBytes = 0;
    uint32_t freeBytes = 0;
    while (last) {
        if (last->free) {
            freeBlocks++;
            freeBytes += last->size;
        } else {
            usedBlocks++;
            usedBytes += last->size;
        }
        blocks++;
        before = last;
        last = last->next;
    }

    print("Current Block Address: ");
    print(hexToString((uintptr_t)before));
    print("\n");
    print("Used blocks: ");
    print(intToString(usedBlocks));
    print("\n");
    print("Free blocks: ");
    print(intToString(freeBlocks));
    print("\n");
    print("Used bytes: ");
    print(intToString(usedBytes));
    print("\n");
    print("Free bytes: ");
    print(intToString(freeBytes));
    print("\n");
}