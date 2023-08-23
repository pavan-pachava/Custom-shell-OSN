#include "header.h"

void bripe(char *command)
{
    int parts = 0, pipe_file[2], prevPipeFile[2], i = 0;
    int stdoutCopy = dup(STDOUT_FILENO);
    int stdinCopy = dup(STDIN_FILENO);
    char **pipes = parse(command, "|", &parts);
    for (i = 0; i < parts - 1; i++)
    {
        pipe(pipe_file);
        if (dup2(pipe_file[1], STDOUT_FILENO) < 0)
        {
            perror("\e[0;91mdup2");
            fprintf(stderr, "\e[0m");
            return;
        }
        close(pipe_file[1]);
        command_process(pipes[i]);
        if (dup2(pipe_file[0], STDIN_FILENO) < 0)
        {
            perror("\e[0;91mdup2");
            fprintf(stderr, "\e[0m");
            return;
        }
        close(pipe_file[0]);
    }
    if (dup2(stdoutCopy, STDOUT_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdoutCopy);
    command_process(pipes[i]);
    if (dup2(stdinCopy, STDIN_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdinCopy);
}