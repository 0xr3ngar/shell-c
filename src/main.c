#include "commands.h"
#include "input.h"
#include "path_search.h"
#include "signals.h"
#include "tokenizer.h"
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
        setbuf(stdout, NULL);

        struct sigaction sa;
        sa.sa_handler = on_sigint;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGINT, &sa, NULL) == -1) {
                return 1;
        }

        while (!stop) {
                char *userInput;

                printf("$ ");

                userInput = getInput(stdin, 200);

                if (userInput == NULL || stop) {
                        if (userInput)
                                free(userInput);
                        break;
                }

                if (userInput[0] == '\0') {
                        free(userInput);
                        continue;
                }

                Tokens tok = getTokens(userInput);

                int fullArgC = tok.tokenCount;

                char **args = malloc(sizeof(char *) * (fullArgC + 1));
                if (!args) {
                        free(userInput);
                        freeTokens(&tok);
                        return 1;
                }

                for (int i = 0; i < fullArgC; i++) {
                        args[i] = tok.tokens[i];
                }
                args[fullArgC] = NULL;

                ParsedCommand pc = {
                    .kind = tok.tokenCount == 0 ? CMD_UNKNOWN
                                                : parseCommand(tok.tokens[0]),
                    .raw = userInput,
                    .cmd = tok.tokenCount == 0 ? NULL : tok.tokens[0],
                    .argc = fullArgC,
                    .args = args,
                };

                switch (pc.kind) {
                case CMD_EXIT:
                        stop = 1;
                        break;
                case CMD_ECHO:
                        printEcho(pc.args + 1, pc.argc - 1);
                        break;
                case CMD_PWD: {
                        char *cwd = getcwd(NULL, 0);
                        if (cwd) {
                                printf("%s\n", cwd);
                                free(cwd);
                                break;
                        }
                        printf("pwd: getcwd: %s\n", cwd);
                        break;
                }
                case CMD_TYPE: {
                        if (tok.tokenCount < 2) {
                                printf("type: missing argument\n");
                                break;
                        }

                        char *arg = pc.args[1];
                        RootCommand arg2 = parseCommand(arg);
                        if (arg2 != CMD_UNKNOWN) {
                                printf("%s is a shell builtin\n", arg);
                                break;
                        }

                        char *path = findExecutableInPath(arg);
                        if (path) {
                                printf("%s is %s\n", arg, path);
                                free(path);
                                break;
                        }

                        printf("%s: not found\n", arg);
                        free(path);
                        break;
                }
                case CMD_UNKNOWN: {
                        char *cmd = pc.args[0];
                        char *path = findExecutableInPath(cmd);
                        if (path) {
                                runCommand(path, pc.args);
                                free(path);
                                break;
                        }

                        printf("%s: not found\n", cmd);
                        free(path);
                        break;
                }
                default:
                        printf("%s: command not found\n", pc.cmd ? pc.cmd : "");
                        break;
                }

                free(userInput);
                freeTokens(&tok);
                free(args);
        }

        return 0;
}
