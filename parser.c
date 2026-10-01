#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int parser(token_list_t *tokens, pipeline_t *pipeline)
{
    if (tokens == NULL || pipeline == NULL)
        return 0;

    pipeline->command_count = 0;

    if (tokens->count == 0)
        return 0;

    int command_index = 0;
    int arg_index = 0;

    for (int i = 0; i < tokens->count; i++)
    {
        char *token = tokens->items[i];

        if (strcmp(token, "|") == 0)
        {
            if (arg_index == 0)
                return 0;

            pipeline->commands[command_index].argv[arg_index] = NULL;
            pipeline->commands[command_index].argc = arg_index;

            command_index++;

            if (command_index >= MAX_COMMANDS)
                return 0;

            arg_index = 0;
        }
        else
        {
            if (arg_index >= MAX_ARGS - 1)
                return 0;

            pipeline->commands[command_index].argv[arg_index] =
                strdup(token);

            arg_index++;
        }
    }

    if (arg_index == 0)
        return 0;

    pipeline->commands[command_index].argv[arg_index] = NULL;
    pipeline->commands[command_index].argc = arg_index;

    pipeline->command_count = command_index + 1;

    return 1;
}

void pipeline_free(pipeline_t *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->command_count; i++)
    {
        for (int j = 0; j < pipeline->commands[i].argc; j++)
        {
            free(pipeline->commands[i].argv[j]);
            pipeline->commands[i].argv[j] = NULL;
        }

        pipeline->commands[i].argc = 0;
    }

    pipeline->command_count = 0;
}

void pipeline_print(pipeline_t *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->command_count; i++)
    {
        printf("Command %d: ", i + 1);

        for (int j = 0; j < pipeline->commands[i].argc; j++)
            printf("%s ", pipeline->commands[i].argv[j]);

        printf("\n");
    }
}

