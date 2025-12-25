#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "tokenizer.h"

static const char *COMMANDS[CMD_COUNT] = {"exit", "echo"};

RootCommand parseCommand(char *s) {
        if (s == NULL) {
                return CMD_UNKNOWN;
        }
        for (int i = 0; i < CMD_COUNT; i++) {
                if (strcmp(s, COMMANDS[i]) == 0) {
                        return (RootCommand)i;
                }
        }
        return CMD_UNKNOWN;
}

void printEcho(char *cmd) {
        const char *command_copy = view_from_index(cmd, 4);

        if (command_copy) {
                printf("copy: %s", command_copy);
                printf("\n");
        }
}

