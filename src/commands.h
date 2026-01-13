#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

typedef enum {
        CMD_EXIT,
        CMD_ECHO,
        CMD_TYPE,
        CMD_COUNT,
        CMD_UNKNOWN = -1
} RootCommand;

typedef struct {
        RootCommand kind;
        char *raw;
        char *cmd;
        char **args;
        size_t argc;
} ParsedCommand;

RootCommand parseCommand(char *s);
void printEcho(char **cmds, size_t size);
int runCommand(char *path, char **argv);

#endif
