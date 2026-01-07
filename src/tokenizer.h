#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stddef.h>
#include <stdio.h>

typedef struct {
        char **tokens;
        size_t tokenCount;
} Tokens;

char *concat(const char *s1, const char *s2);
Tokens getTokens(char *userInput);
const char *view_from_index(const char *src, size_t n);
char *trim(char *s);
size_t split_string(const char *s, char ***outTokens,
                    const char *filteredCharacter);
void freeTokens(Tokens *t);

#endif
