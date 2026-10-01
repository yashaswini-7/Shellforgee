#include <stdio.h>
#include <stdlib.h>
#include "token.h"

void token_free(token_list_t *tokens)
{
    if (tokens == NULL)
        return;

    for (int i = 0; i < tokens->count; i++)
        free(tokens->items[i]);

    free(tokens->items);

    tokens->items = NULL;
    tokens->count = 0;
}

void token_print(token_list_t *tokens)
{
    if (tokens == NULL)
        return;

    for (int i = 0; i < tokens->count; i++)
        printf("[%s]\n", tokens->items[i]);
}
