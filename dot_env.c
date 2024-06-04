#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <sys/types.h>
#endif

#define ENV_FILE_NAME ".env"
#define COMMENT_CHAR  '#'
#define SEPERATOR     "="
#define NEW_LINE      "\n"


int
env_load(const char *path, const int overwrite)
{
    char full_path[strlen(path) + strlen(ENV_FILE_NAME) + 1];
    
    if (path == NULL || path[0] == '\0') {
        strcpy(full_path, ENV_FILE_NAME);
    } else {
        strcpy(full_path, path);
    }

    FILE* f = fopen(full_path, "rb");
    if (f == NULL) {
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, f)) != -1) {
        // ignore lines starting with # and whitespace
        if (COMMENT_CHAR == line[0] || isspace(line[0])) {
            continue;
        }
        
        char *key = strsep(&line, SEPERATOR);
        char *value = strsep(&line, NEW_LINE);

        replace_double_quotes(value);

        if (setenv(key, trim_whitespace(value), overwrite) != 0) {
            fclose(f);
            free(line);
            return 1;
        }
    }

    fclose(f);

    return 0;
}
