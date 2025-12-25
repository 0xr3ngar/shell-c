#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile sig_atomic_t stop = 0;

void on_sigint(int signum) {
        (void)signum;
        stop = 1;
        printf("\n");
}

char *get_input(FILE *fp, size_t size) {
        char *str;
        int ch;
        size_t len = 0;
        str = realloc(NULL, sizeof(*str) * size);
        if (!str)
                return str;

        while (EOF != (ch = fgetc(fp)) && ch != '\n') {
                str[len++] = ch;
                if (len == size) {
                        str = realloc(str, sizeof(*str) * (size += 16));
                        if (!str)
                                return str;
                }
        }

        if (ch == EOF && errno == EINTR && stop) {
                free(str);
                return NULL;
        }

        if (ch == EOF && len == 0) {
                free(str);
                return NULL;
        }

        str[len++] = '\0';

        return realloc(str, sizeof(*str) * len);
}

typedef enum { CMD_EXIT, CMD_COUNT, CMD_UNKNOWN = -1 } Command;

static const char *COMMANDS[CMD_COUNT] = {"exit"};

static Command parse_command(char *s) {
        if (s == NULL) {
                return CMD_UNKNOWN;
        }
        for (int i = 0; i < CMD_COUNT; i++) {
                if (strcmp(s, COMMANDS[i]) == 0) {
                        return (Command)i;
                }
        }
        return CMD_UNKNOWN;
}

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

                userInput = get_input(stdin, 20);

                if (userInput == NULL || stop) {
                        if (userInput)
                                free(userInput);
                        break;
                }

                if (userInput[0] == '\0') {
                        free(userInput);
                        continue;
                }

                Command cmd = parse_command(userInput);

                switch (cmd) {
                case CMD_EXIT:
                        stop = 1;
                        break;
                default:
                        printf("%s: command not found\n", userInput);
                        break;
                }

                free(userInput);
        }

        return 0;
}
