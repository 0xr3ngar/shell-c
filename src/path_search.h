#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H

/**
 * Searches for an executable file in the directories specified by the PATH
 * environment variable.
 *
 * This function iterates through each directory in PATH and checks if the
 * specified command exists as an executable file.
 *
 * @param command The command name to search for (e.g., "ls", "grep")
 * @return A newly allocated string containing the full path to the executable,
 *         or NULL if not found or on error
 *
 * The function will:
 * 1. Get the PATH environment variable
 * 2. Split it by ':' delimiter
 * 3. For each directory, construct the full path
 * 4. Check if the file exists and has execute permissions
 * 5. Return the first match found
 *
 * Note: Caller is responsible for freeing the returned string.
 */
char *find_executable_in_path(const char *command);

#endif
