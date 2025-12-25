#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "input.h"
#include "signals.h"
#include "tokenizer.h"

// TODO: tokenize
// 1. Trim leading/trailing whitespace

// 2. Scan characters, building tokens

// 3. Return: command = first token, args = remaining tokens in array

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

                char **tokens = getTokens(userInput);

                //          if tokens.count == 0 : pc = {
                //         kind : CMD_UNKNOWN,
                //         raw : line,
                //         cmd : null,
                //         args : null,
                //         argc : 0
                // } else : pc = {
                //         kind : parseCommand(tokens[0]),
                //         raw : line,
                //         cmd : tokens[0],
                //         args : tokens[1..],
                //         argc : tokens.count - 1
                // }

                RootCommand cmd = parseCommand(userInput); // Temporary until tokenization is done
                switch (cmd) {
                case CMD_EXIT:
                        stop = 1;
                        break;
                case CMD_ECHO:
                        printf("str: %s, \n", userInput);
                        printEcho(userInput);
                        break;
                default:
                        printf("%s: command not found\n", userInput);
                        break;
                }

                free(userInput);
        }

        return 0;
}
