#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

int is_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "export") == 0)
        return 1;

    return 0;
}

int execute_builtin(command_t *cmd)
{
    if (strcmp(cmd->argv[0], "cd") == 0)
    {
        const char *path;

        if (cmd->argc > 1)
            path = cmd->argv[1];
        else
            path = getenv("HOME");

        if (chdir(path) != 0)
        {
            perror("cd");
            return 1;
        }

        return 0;
    }

    if (strcmp(cmd->argv[0], "pwd") == 0)
    {
        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");

        return 0;
    }

    if (strcmp(cmd->argv[0], "echo") == 0)
    {
        for (int i = 1; i < cmd->argc; i++)
        {
            printf("%s", cmd->argv[i]);

            if (i < cmd->argc - 1)
                printf(" ");
        }

        printf("\n");
        return 0;
    }

    if (strcmp(cmd->argv[0], "export") == 0)
    {
        if (cmd->argc < 2)
            return 0;

        char *equal = strchr(cmd->argv[1], '=');

        if (equal == NULL)
        {
            printf("export: use NAME=value\n");
            return 1;
        }

        *equal = '\0';

        if (setenv(cmd->argv[1], equal + 1, 1) != 0)
        {
            perror("export");
            return 1;
        }

        return 0;
    }

    return 1;
}
