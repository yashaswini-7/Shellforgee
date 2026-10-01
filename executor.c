#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "executor.h"
#include "builtin.h"

int execute_command(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 1;

    if (is_builtin(cmd))
        return execute_builtin(cmd);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        execvp(cmd->argv[0], cmd->argv);

        perror(cmd->argv[0]);
        _exit(127);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return 1;
}

int execute_pipeline(pipeline_t *pipeline)
{
    if (pipeline == NULL || pipeline->command_count == 0)
        return 1;

    if (pipeline->command_count == 1)
        return execute_command(&pipeline->commands[0]);

    int previous_read = -1;

    pid_t pids[MAX_COMMANDS];

    for (int i = 0; i < pipeline->command_count; i++)
    {
        int pipefd[2];

        if (i < pipeline->command_count - 1)
        {
            if (pipe(pipefd) < 0)
            {
                perror("pipe");
                return 1;
            }
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            if (previous_read != -1)
            {
                if (dup2(previous_read, STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    _exit(1);
                }
            }

            if (i < pipeline->command_count - 1)
            {
                if (dup2(pipefd[1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    _exit(1);
                }
            }

            if (previous_read != -1)
                close(previous_read);

            if (i < pipeline->command_count - 1)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }

            command_t *cmd = &pipeline->commands[i];

            if (is_builtin(cmd))
                _exit(execute_builtin(cmd));

            execvp(cmd->argv[0], cmd->argv);

            perror(cmd->argv[0]);
            _exit(127);
        }

        pids[i] = pid;

        if (previous_read != -1)
            close(previous_read);

        if (i < pipeline->command_count - 1)
        {
            close(pipefd[1]);
            previous_read = pipefd[0];
        }
    }

    if (previous_read != -1)
        close(previous_read);

    int last_status = 0;

    for (int i = 0; i < pipeline->command_count; i++)
    {
        int status;

        if (waitpid(pids[i], &status, 0) < 0)
        {
            perror("waitpid");
            continue;
        }

        if (i == pipeline->command_count - 1)
            last_status = status;
    }

    if (WIFEXITED(last_status))
        return WEXITSTATUS(last_status);

    return 1;
}
