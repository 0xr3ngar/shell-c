// saw this from tsoding :chad:
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

/**
 * Concatenates two strings and returns a newly allocated string.
 * @param s1 First string (must not be NULL)
 * @param s2 Second string (must not be NULL)
 * @return Newly allocated string containing s1 + s2, or NULL on allocation
 * failure
 *
 * Caller is responsible for freeing the returned string.
 */
char *string_concat(const char *s1, const char *s2);

/**
 * Builds a file path by joining directory and filename with a separator.
 * Automatically handles the path separator ('/').
 *
 * @param dir Directory path (must not be NULL)
 * @param filename Filename to append (must not be NULL)
 * @return Newly allocated string containing the full path, or NULL on
 * allocation failure
 *
 * Example: build_path("/usr/bin", "ls") returns "/usr/bin/ls"
 *
 * Caller is responsible for freeing the returned string.
 */
char *build_path(const char *dir, const char *filename);

#endif
