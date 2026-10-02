#include <stdlib.h>
#include <string.h>

#include "expand.h"

void expand_variables(pipeline_t *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->command_count; i++)
    {
        command_t *cmd = &pipeline->commands[i];

        for (int j = 0; j < cmd->argc; j++)
        {
            char *arg = cmd->argv[j];

            if (arg == NULL)
                continue;

            if (arg[0] == '$')
            {
                const char *value = getenv(arg + 1);

                if (value != NULL)
                {
                    char *new_value = malloc(strlen(value) + 1);

                    if (new_value != NULL)
                    {
                        strcpy(new_value, value);
                        free(cmd->argv[j]);
                        cmd->argv[j] = new_value;
                    }
                }
            }
        }
    }
}
