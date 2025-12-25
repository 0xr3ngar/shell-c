#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "signals.h"

char *getInput(FILE *fp, size_t size) {
        char *str;
        int ch;
        size_t len = 0;
        str = realloc(NULL, sizeof(*str) * size);
        if (!str) {
                return str;
        }

        while (EOF != (ch = fgetc(fp)) && ch != '\n') {
                str[len++] = ch;
                if (len == size) {
                        str = realloc(str, sizeof(*str) * (size += 16));
                        if (!str) {
                                return str;
                        }
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

