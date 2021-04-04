#include <stdio.h>

#include "../dot_env.h"

int
main(int argc, char **argv)
{
    env_load("./config.env", 0);

    printf("server: %s\n", getenv("SERVER"));
    printf("user: %s\n", getenv("USER"));
    printf("extra_var: %s\n", getenv("EXTRA_VAR"));
    printf("empty: %s\n", getenv("EMPTY"));
    printf("double: %s\n", getenv("DOUBLE_QUOTES"));
    printf("single: %s\n", getenv("SINGLE_QUOTES"));
    printf("inner space: %s\n", getenv("INNER_SPACE"));
    printf("outer space: %s\n", getenv("OUTER_SPACE"));
    printf("multi line: %s\n", getenv("MULTILINE"));

    printf("multi line: %s\n", getenv("DB_USER"));

    return 0;
}