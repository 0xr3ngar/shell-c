#include "path_search.h"
#include "string_utils.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *find_executable_in_path(const char *command) {
        if (!command || command[0] == '\0') {
                return NULL;
        }

        const char *path_env = getenv("PATH");
        if (!path_env) {
                return NULL;
        }

        char **directories = NULL;
        size_t dir_count = split_string(path_env, &directories, ':');

        if (!directories || dir_count == 0) {
                return NULL;
        }

        char *result = NULL;

        for (size_t i = 0; i < dir_count; i++) {
                char *full_path = build_path(directories[i], command);

                if (!full_path) {
                        continue;
                }

                if (access(full_path, X_OK) == 0) {
                        result = full_path;
                        break;
                }

                free(full_path);
        }

        for (size_t i = 0; i < dir_count; i++) {
                free(directories[i]);
        }
        free(directories);

        return result;
}
