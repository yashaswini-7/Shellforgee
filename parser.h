#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 32
#define MAX_ARGS 64

typedef struct {
    int argc;
    char *argv[MAX_ARGS];
} command_t;

typedef struct {
    int command_count;
    command_t commands[MAX_COMMANDS];
} pipeline_t;

int parser(token_list_t *tokens, pipeline_t *pipeline);
void pipeline_free(pipeline_t *pipeline);
void pipeline_print(pipeline_t *pipeline);

#endif
