#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 50
#define MAX_ARGS 100

typedef struct
{
    int argc;
    char *argv[MAX_ARGS + 1];

    char input[256];
    char output[256];

    int append;
    int background;
} command_t;

typedef struct
{
    int command_count;
    command_t commands[MAX_COMMANDS];
    int background;
} pipeline_t;

int parser(const token_list_t *tokens, pipeline_t *pipeline);

void free_pipeline(pipeline_t *pipeline);

#endif
