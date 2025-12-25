#include <stdio.h>
#include <stdlib.h>

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
  str[len++] = '\0';

  return realloc(str, sizeof(*str) * len);
}

int main(void) {
  setbuf(stdout, NULL);

  char *userInput;

  printf("$ ");

  userInput = get_input(stdin, 20);

  printf("%s: command not found\n", userInput);

  free(userInput);
  return 0;
}
