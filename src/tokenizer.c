#include <string.h>
#include "tokenizer.h"

const char *view_from_index(const char *src, size_t n) {
        if (!src) {
                return NULL;
        }

        size_t len = strlen(src);
        if (n > len) {
                return NULL;
        }

        return src + n;
}

char **getTokens(char *userInput) {}

