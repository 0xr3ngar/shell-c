#include "commands.h"
#include "input.h"
#include "signals.h"
#include "tokenizer.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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
