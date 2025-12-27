#include "commands.h"
#include <stdio.h>
#include <string.h>

static const char *COMMANDS[CMD_COUNT] = {"exit", "echo", "type"};

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

void printEcho(char **cmds, size_t size) {
        for (int i = 0; i < size; i++) {
                printf("%s ", cmds[i]);
        }
        printf("\n");
}
