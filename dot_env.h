/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _DOT_ENV_H
#define _DOT_ENV_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENV_FILE_NAME ".env"
#define COMMENT_CHAR  '#'
#define SEPERATOR     "="
#define NEW_LINE      "\n"

static char*
trim_whitespace(char *str)
{
    char *end;

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    end[1] = '\0';

    return str;
}

static int
replace_double_quotes(char *value)
{
    if (value == NULL || strlen(value) == 0) {
        return 0;
    }

    char last = strlen(value)-1;
    if (value[0] == '\'' && value[last] == '\'') {
        value[0] = '"';
        value[last] = '"';
    }

    return 0;
}

/**
 * env_load loads all entries in the found .env. If a path is
 * provided, the .env file is looked for there. An option to 
 * overwrite existing values can be provided by passing a value 
 * greater than 0. Otherwise the value isn't overwritten.
 * 
 * A 0 is returned on success and values can be retrieved from 
 * the environment. A 1 is returned on error. errno should be 
 * checked for further detail.
 */
int
env_load(const char *path, const int overwrite)
{
    char full_path[strlen(path) + strlen(ENV_FILE_NAME) + 1];
    
    if (path == NULL || path[0] == '\0') {
        strcpy(full_path, ENV_FILE_NAME);
    } else {
        sprintf(full_path, "%s/.env", path);
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

        char *token = strdup(line);
        char *key = strsep(&token, SEPERATOR);
        char *value = strsep(&token, NEW_LINE);

        replace_double_quotes(value);

        if (setenv(key, trim_whitespace(value), overwrite) != 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    free(line);

    return 0;
}

#endif
