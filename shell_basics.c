#include "header.h"


void shell_setup()
{
    gethostname(systemname, 1024);
    strcpy(username, getenv("USER"));
    getcwd(home_dir, 1024);
    parent = getpid();
    history_retrieval();
}

void shell_prompt()
{
    char cwd[1024];
    getcwd(cwd, 1024);
    rel_path(cwd);
    printf("\e[0;96m<%s@%s:\e[1;34m%s\e[0;96m> \e[0m", username, systemname, cwd);
    fflush(stdout);
}

void child_terminate(int signal)
{
    int status;
    pid_t wpid = waitpid(-1, &status, WNOHANG);
    if (wpid > 0 && wpid != fg_proc)
    {
        char *name = get_process_name(wpid);
        if (WIFEXITED(status))
        {
            fprintf(stderr, "\n\e[0;32m%s with pid %d exited normally\e[0m\n", name, wpid);
        }
        else
        {
            fprintf(stderr, "\n\e[0;91m%s with pid %d exited abnormally\e[0m\n", name, wpid);
        }
        free(name);
        remove_process(wpid);
        shell_prompt();
    }
}

void fg_to_bg_handler(int signal)
{
    if (fg_proc != 0)
    {
        add_bgp_node(fg_proc, fg_proc_name);
        kill(fg_proc, 20);
        printf("\r");
    }
}

void ctrl_C_handler(int signal)
{
    if (fg_proc != 0)
    {
        kill(fg_proc, 2);
        printf("\r");
    }
}

void shell()
{
    char individual_command[1024];
    char *input;
    char *commands;
    int i = 0, no_of_commands;
    size_t input_size = 0;
    shell_setup();


    strcpy(prev_dir, home_dir);
    while (1)
    {
        i = 0;
        shell_prompt();
        input = input_line();
        if (strcmp(input, "") != 0)
        {
            if (his_start == NULL)
            {
                add_his_node(input);
                history_storage();
            }
            else if (strcmp(input, his_start->command) != 0)
            {
                add_his_node(input);
                history_storage();
            }
        }
        remove_spaces(input);

        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        char **commands = parse(input, ";", &no_of_commands);
        for (i = 0; i < no_of_commands; i++)
        {
            bripe(commands[i]);
        }
        free_memory(&commands, no_of_commands);
    }
}