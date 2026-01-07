#include "commands.h"
#include "input.h"
#include "signals.h"
#include "tokenizer.h"
#include <dirent.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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

                char **args = malloc(sizeof(char *) * (tok.tokenCount - 1));
                if (!args) {
                        free(userInput);
                        free(args);
                        freeTokens(&tok);
                        return 1;
                }
                for (int i = 1; i < tok.tokenCount; i++) {
                        args[i - 1] = tok.tokens[i];
                }

                ParsedCommand pc = {
                    .kind = tok.tokenCount == 0 ? CMD_UNKNOWN
                                                : parseCommand(tok.tokens[0]),
                    .raw = userInput,
                    .cmd = tok.tokenCount == 0 ? NULL : tok.tokens[0],
                    .argc = tok.tokenCount == 0 ? 0 : tok.tokenCount - 1,
                    .args = args,
                };

                switch (pc.kind) {
                case CMD_EXIT:
                        stop = 1;
                        break;
                case CMD_ECHO:
                        printEcho(pc.args, pc.argc);
                        break;
                case CMD_TYPE: {
                        RootCommand arg2 = parseCommand(tok.tokens[1]);
                        if (arg2 == CMD_UNKNOWN) {
                                const char *pathEnv = getenv("PATH");
                                if (pathEnv == NULL) {
                                        printf("PATH env variable is not "
                                               "defined\n");
                                        printf("%s: not found\n",
                                               tok.tokens[1]);
                                        break;
                                }

                                char **dir = NULL;

                                size_t n = split_string(pathEnv, &dir, ":");

                                Tokens allDirs = {.tokenCount = n,
                                                  .tokens = dir};

                                int found = 0;
                                for (int i = 0;
                                     i < allDirs.tokenCount && !found; i++) {
                                        DIR *dp = opendir(allDirs.tokens[i]);
                                        if (dp == NULL) {
                                                perror("Couldn't open the "
                                                       "directory");
                                                continue;
                                        }

                                        struct dirent *ep;
                                        while ((ep = readdir(dp)) != NULL) {
                                                int isNameFound = strcmp(
                                                    ep->d_name, tok.tokens[1]);

                                                if (isNameFound != 0) {
                                                        continue;
                                                }

                                                char *fileDir = concat(
                                                    allDirs.tokens[i], "/");

                                                char *file =
                                                    concat(fileDir, ep->d_name);

                                                int canExecute =
                                                    access(file, X_OK);

                                                if (canExecute != 0) {
                                                        free(fileDir);
                                                        free(file);
                                                        continue;
                                                }

                                                printf("%s is "
                                                       "%s\n",
                                                       tok.tokens[1],
                                                       allDirs.tokens[i]);

                                                found = 1;
                                                free(fileDir);
                                                free(file);
                                                break;
                                        }

                                        closedir(dp);

                                        if (found) {
                                                break;
                                        }
                                }

                                if (found)
                                        break;

                                printf("%s: not found\n", tok.tokens[1]);
                                break;
                        }

                        printf("%s is a shell builtin\n", tok.tokens[1]);
                        break;
                }
                default:
                        printf("%s: command not found\n", userInput);
                        break;
                }

                free(userInput);
                freeTokens(&tok);
                free(args);
        }

        return 0;
}
