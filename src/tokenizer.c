#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char *trim(char *s) {
        if (!s) {
                return NULL;
        }

        unsigned char *start = (unsigned char *)s;
        while (*start && isspace(*start))
                start++;

        unsigned char *end =
            (unsigned char *)start + strlen((const char *)start);

        while (end > start && isspace(*(end - 1)))
                end--;

        *end = '\0';
        return (char *)start;
}

size_t split_whitespace(const char *s, char ***outTokens) {
        if (!outTokens)
                return 0;
        *outTokens = NULL;
        if (!s)
                return 0;

        const unsigned char *p = (const unsigned char *)s;
        size_t cap = 8;
        size_t count = 0;
        char **tokens = (char **)malloc(cap * sizeof(char *));
        if (!tokens)
                return 0;

        while (*p) {
                while (*p && isspace(*p))
                        p++;
                if (!*p)
                        break;

                const unsigned char *start = p;
                while (*p && !isspace(*p))
                        p++;
                size_t len = (size_t)(p - start);

                char *tok = (char *)malloc(len + 1);
                if (!tok) {
                        // Cleanup on allocation failure
                        for (size_t i = 0; i < count; i++)
                                free(tokens[i]);
                        free(tokens);
                        return 0;
                }
                memcpy(tok, start, len);
                tok[len] = '\0';

                if (count == cap) {
                        size_t newCap = cap * 2;
                        char **nt =
                            (char **)realloc(tokens, newCap * sizeof(char *));
                        if (!nt) {
                                free(tok);
                                for (size_t i = 0; i < count; i++)
                                        free(tokens[i]);
                                free(tokens);
                                return 0;
                        }
                        tokens = nt;
                        cap = newCap;
                }

                tokens[count++] = tok;
        }

        *outTokens = tokens;
        return count;
}

// example input "   echo "
Tokens getTokens(char *userInput) {
        // first step clear white spaces and find a character
        char *trimmedInput = trim(userInput);

        char **tok = NULL;
        size_t n = split_whitespace(trimmedInput, &tok);

        Tokens allTokens = {.tokenCount = n, .tokens = tok};

        return allTokens;
}

void freeTokens(Tokens *t) {
        if (!t || !t->tokens)
                return;
        for (size_t i = 0; i < t->tokenCount; i++) {
                free(t->tokens[i]);
        }
        free(t->tokens);
        t->tokens = NULL;
        t->tokenCount = 0;
}
