#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int lexer(const char *line, token_list_t *tokens)
{
    if (line == NULL || tokens == NULL)
        return 0;

    tokens->items = NULL;
    tokens->count = 0;

    char *copy = strdup(line);

    if (copy == NULL)
        return 0;

    int capacity = 10;

    tokens->items = malloc(sizeof(char *) * capacity);

    if (tokens->items == NULL)
    {
        free(copy);
        return 0;
    }

    char *p = copy;

    while (*p != '\0')
    {
        while (*p == ' ' || *p == '\t')
            p++;

        if (*p == '\0')
            break;

        if (*p == '|')
        {
            if (tokens->count >= capacity)
            {
                capacity *= 2;
                tokens->items = realloc(
                    tokens->items,
                    sizeof(char *) * capacity
                );
            }

            tokens->items[tokens->count++] = strdup("|");
            p++;
            continue;
        }

        char buffer[1024];
        int i = 0;

        while (*p != '\0' &&
               *p != ' ' &&
               *p != '\t' &&
               *p != '|')
        {
            buffer[i++] = *p;
            p++;
        }

        buffer[i] = '\0';

        if (tokens->count >= capacity)
        {
            capacity *= 2;

            tokens->items = realloc(
                tokens->items,
                sizeof(char *) * capacity
            );
        }

        tokens->items[tokens->count++] = strdup(buffer);
    }

    free(copy);

    return 1;
}
