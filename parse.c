#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "header.h"

char **parse(char string[], char delimiter[], int *no_parts)
{
    char *part, *save_ptr;
    char temp[1024];
    int i = 0;
    strcpy(temp, string);
    *no_parts = 0;
    for (part = strtok_r(temp, delimiter, &save_ptr) ; part != NULL ; part = strtok_r(NULL, delimiter, &save_ptr))
    {
        (*no_parts)++;
    }
    strcpy(temp, string);
    char **parts = (char **)calloc((*no_parts + 1), sizeof(char *));
    part = string;

    for (part = strtok_r(temp, delimiter, &save_ptr) ; part != NULL ; part = strtok_r(NULL, delimiter, &save_ptr))
    {
        parts[i] = (char *)calloc(strlen(part) + 2, sizeof(char));
        strcpy(parts[i], part);
        i++;
    }
    parts[*no_parts] = NULL;
    return parts;
}

void free_memory(char ***mem, int no)
{
    for (int i = no; i >= 0; i--)
    {
        free((*mem)[i]);
    }
    free(*mem);
}