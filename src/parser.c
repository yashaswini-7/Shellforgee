#include <stdlib.h>
#include <string.h>

#include "parser.h"

int parser(const token_list_t *tokens, pipeline_t *pipeline)
{
    if (tokens == NULL || pipeline == NULL)
        return 0;

    pipeline->command_count = 0;
    pipeline->background = 0;

    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        pipeline->commands[i].argc = 0;
        pipeline->commands[i].input[0] = '\0';
        pipeline->commands[i].output[0] = '\0';
        pipeline->commands[i].append = 0;
        pipeline->commands[i].background = 0;
    }

    int command_index = 0;

    for (int i = 0; i < tokens->count; i++)
    {
        token_t token = tokens->tokens[i];
        command_t *cmd = &pipeline->commands[command_index];

        if (token.type == TOKEN_END)
            break;

        if (token.type == TOKEN_WORD)
        {
            if (cmd->argc >= MAX_ARGS)
                return 0;

            cmd->argv[cmd->argc] = malloc(strlen(token.text) + 1);

            if (cmd->argv[cmd->argc] == NULL)
                return 0;

            strcpy(cmd->argv[cmd->argc], token.text);
            cmd->argc++;
            cmd->argv[cmd->argc] = NULL;
        }
        else if (token.type == TOKEN_PIPE)
        {
            if (cmd->argc == 0)
                return 0;

            command_index++;

            if (command_index >= MAX_COMMANDS)
                return 0;
        }
        else if (token.type == TOKEN_INPUT)
        {
            i++;

            if (i >= tokens->count ||
                tokens->tokens[i].type != TOKEN_WORD)
                return 0;

            strcpy(cmd->input, tokens->tokens[i].text);
        }
        else if (token.type == TOKEN_OUTPUT ||
                 token.type == TOKEN_APPEND)
        {
            i++;

            if (i >= tokens->count ||
                tokens->tokens[i].type != TOKEN_WORD)
                return 0;

            strcpy(cmd->output, tokens->tokens[i].text);

            if (token.type == TOKEN_APPEND)
                cmd->append = 1;
            else
                cmd->append = 0;
        }
        else if (token.type == TOKEN_BACKGROUND)
        {
            cmd->background = 1;
            pipeline->background = 1;
        }
    }

    if (pipeline->commands[command_index].argc == 0)
        return 0;

    pipeline->command_count = command_index + 1;

    return 1;
}

void free_pipeline(pipeline_t *pipeline)
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
