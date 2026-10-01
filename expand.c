#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expand.h"

void expand_variables(pipeline_t *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->command_count; i++)
    {
        for (int j = 0; j < pipeline->commands[i].argc; j++)
        {
            char *arg = pipeline->commands[i].argv[j];

            if (arg[0] != '$')
                continue;

            char *value = getenv(arg + 1);

            if (value != NULL)
            {
                free(pipeline->commands[i].argv[j]);
                pipeline->commands[i].argv[j] = strdup(value);
            }
        }
    }
}
