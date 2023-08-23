#include "header.h"

void cd_com(char **argument)
{
    
    if (strcmp(argument[1], "-") == 0)
    {
        char temp[1024];
        getcwd(temp, 1024);
        if (colorCode == 0)
        {
            printf("\e[0;32m");
        }
        printf("%s\n", prev_dir);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
        if (chdir(prev_dir) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
        strcpy(prev_dir, temp);
    }
    else if (strcmp(argument[1], "~") == 0 || argument[1] == NULL)
    {
        getcwd(prev_dir, 1024);
        if (chdir(home_dir) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
    }
    else
    {
        getcwd(prev_dir, 1024);
        if (chdir(argument[1]) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
    }
}

void pwd_com()
{
    char temp[1024];
    getcwd(temp, 1024);
    printf("%s\n", temp);
}

void echo_com(char **argument, int parts)
{
    int i;
    for (i = 1; i < parts; i++)
    {
        printf("%s ", argument[i]);
    }
    printf("\n");
}

void bg_com(char **argument, int parts)
{
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(argument[0], argument) == -1)
        {
            fprintf(stderr, "\e[0;91m");
            perror("Shell");
            fprintf(stderr, "\e[0m");
        }
        exit(0);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "\e[0;91m");
        perror("Fork");
        fprintf(stderr, "\e[0m");
    }
    else
    {
        if (colorCode == 0)
        {
            printf("\e[4;32m");
        }
        printf("%d\n", pid);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
        char *pname = combination(argument, 0, parts);
        add_bgp_node(pid, pname);
        free(pname);
    }
}

void fg_com(char **argument, int parts)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        if (execvp(argument[0], argument) == -1)
        {
            fprintf(stderr, "\e[0;91m");
            perror("Shell");
            fprintf(stderr, "\e[0m");
        }
        exit(1);
    }
    else if (pid < 0)
    {
        perror("Fork");
    }
    else
    {
        fg_proc = pid;
        fg_proc_name = combination(argument, 0, parts);
        waitpid(pid, &status, WUNTRACED);
        fg_proc = 0;
        free(fg_proc_name);
        fg_proc_name = NULL;
    }
}

void make_fg_com(int proc)
{
    bgp *traversal_node = bgp_start;
    int status;
    while (traversal_node != NULL)
    {
        if (traversal_node->pos == proc)
        {
            int pid = traversal_node->pid;
            kill((pid_t)(pid), 18);
            fg_proc = pid;
            fg_proc_name = strdup(traversal_node->process_name);
            if (search_state(fg_proc) != 1)
            {
                remove_process(pid);
                waitpid(pid, &status, WUNTRACED);
            }
            fg_proc = 0;
            free(fg_proc_name);
            fg_proc_name = NULL;
            return;
        }
        traversal_node = traversal_node->next;
    }
    fprintf(stderr, "\e[0;91mNo job with number [%d] exists\e[0m\n", proc);
}

void make_bg_com(int proc)
{
    bgp *traversal_node = bgp_start;
    while (traversal_node != NULL)
    {
        if (traversal_node->pos == proc)
        {
            kill((pid_t)(traversal_node->pid), 18);
            return;
        }
        traversal_node = traversal_node->next;
    }
    fprintf(stderr, "\e[0;91mNo job with number [%d] exists\e[0m\n", proc);
}


void command_process(char command[])
{
    int parts;
    char **argument = parse(command, " ", &parts);
    int stdinCopy = dup(STDIN_FILENO);
    int stdoutCopy = dup(STDOUT_FILENO);
    if (argument != NULL)
    {
        if (strcmp(argument[0], "cd") == 0)
        {
            if (parts > 2)
            {
                fprintf(stderr, "\e[0;91mError: Invalid number of arguments\n\e[0m");
            }
            else
                cd_com(argument);
        }
        else if (strcmp(argument[0], "echo") == 0)
        {
            echo_com(argument, parts);
        }
        else if (strcmp(argument[0], "pwd") == 0)
        {
            pwd_com();
        }
        else if (strcmp(argument[0], "history") == 0)
        {
            if (parts == 1)
                history_command(his_start, 10);
            else
                history_command(his_start, atoi(argument[1]));
        }
        else if (strcmp(argument[0], "pinfo") == 0)
        {
            if (parts == 1)
            {
                pinfo_com(getpid());
            }
            else
            {
                pinfo_com(atoi(argument[1]));
            }
        }
        else if (strcmp(argument[0], "ls") == 0)
        {
            int a = 0, l = 0, no_of_flags = 0, no_of_dir = 0;
            for (int i = 1; i < parts; i++)
            {
                if (strcmp(argument[i], "-l") == 0)
                {
                    no_of_flags++;
                    l = 1;
                }
                else if (strcmp(argument[i], "-a") == 0)
                {
                    no_of_flags++;
                    a = 1;
                }
                else if ((strcmp(argument[i], "-al") == 0) || (strcmp(argument[i], "-la") == 0))
                {
                    no_of_flags++;
                    l = a = 1;
                }
                else if (strcmp(argument[i], "~") == 0)
                {
                    argument[i] = strdup(home_dir);
                }
            }
            if (colorCode == 0)
            {
                printf("\e[0;32m");
            }
            no_of_dir = parts - no_of_flags - 1;
            if (no_of_dir == 0)
            {
                ls_command(a, l, 0, ".");
            }
            else
            {
                for (int i = 1; i < parts; i++)
                {
                    if (strcmp(argument[i], "-al") != 0 && strcmp(argument[i], "-la") != 0 && strcmp(argument[i], "-a") != 0 && strcmp(argument[i], "-l") != 0)
                    {
                        ls_command(a, l, no_of_dir, argument[i]);
                        if (no_of_dir > 1)
                            printf("\n");
                        no_of_dir--;
                    }
                }
            }
            if (colorCode == 0)
            {
                printf("\e[0m");
            }
        }
        else if (strcmp(argument[0], "exit") == 0)
        {
            printf("exit\n");
            exit(0);
        }
        else
        {
            if (strcmp(argument[parts - 1], "&") == 0)
            {
                free(argument[parts - 1]);
                argument[parts - 1] = NULL;
                parts--;
                bg_com(argument, parts);
            }
            else
            {
                fg_com(argument, parts);
            }
        }
    }
    if (dup2(stdinCopy, STDIN_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdinCopy);
    if (dup2(stdoutCopy, STDOUT_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdoutCopy);
    free_memory(&argument, parts);
    colorCode = 0;
}