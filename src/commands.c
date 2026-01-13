#include "commands.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

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

int runCommand(char *path, char **argv) {
        pid_t pid = fork();
        if (pid < 0) {
                perror("fork");
                return -1;
        }
        if (pid == 0) {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                execvp(path, argv);
                perror("execvp");
                _exit(127);
        }
        int status;

        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
        }

        return 128 + WTERMSIG(status);
}
