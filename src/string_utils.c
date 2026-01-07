#include "string_utils.h"
#include <stdlib.h>
#include <string.h>

char *string_concat(const char *s1, const char *s2) {
        if (!s1 || !s2) {
                return NULL;
        }

        size_t len1 = strlen(s1);
        size_t len2 = strlen(s2);

        // +1 for null terminator
        char *result = malloc(len1 + len2 + 1);
        if (!result) {
                return NULL;
        }

        memcpy(result, s1, len1);
        memcpy(result + len1, s2, len2);
        result[len1 + len2] = '\0';

        return result;
}

char *build_path(const char *dir, const char *filename) {
        if (!dir || !filename) {
                return NULL;
        }

        size_t dir_len = strlen(dir);
        size_t filename_len = strlen(filename);

        // check if directory already ends with '/'
        int needs_separator = (dir_len > 0 && dir[dir_len - 1] != '/');

        // +1 for separator (if needed), +1 for null terminator
        size_t total_len =
            dir_len + filename_len + (needs_separator ? 1 : 0) + 1;

        char *result = malloc(total_len);
        if (!result) {
                return NULL;
        }

        char *ptr = result;

        memcpy(ptr, dir, dir_len);
        ptr += dir_len;

        if (needs_separator) {
                *ptr = '/';
                ptr++;
        }

        memcpy(ptr, filename, filename_len);
        ptr += filename_len;

        *ptr = '\0';

        return result;
}
