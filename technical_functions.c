#include "header.h"

void remove_spaces(char string[])
{
    char temp[strlen(string) + 1];
    int i = 0, j = 0;

    while (string[i] != '\0')
    {
        while (string[i + 1] == ' ' && string[i] == ' ')
            i++;

        temp[j] = string[i];
        j++;
        i++;
    }

    i = 0;

    while (string[i] != '\0')
    {
        if (string[i] == '\t')
            string[i] = ' ';
        i++;
    }

    i = 0;

    temp[j] = '\0';
    strcpy(string, temp);
}

char *combination(char **argument, int start_part, int end_part)
{
    char temp[1024] = "";
    for (int i = start_part; i < end_part; i++)
    {
        strcat(temp, argument[i]);
        strcat(temp, " ");
    }
    temp[strlen(temp) - 1] = '\0';
    return strdup(temp);
}

void backspace(char str[], int *position)
{
    if (*position > 0)
    {
        if (str[(*position) - 1] == 9)
        {
            for (int i = 0; i < 7; i++)
            {
                printf("\b");
            }
        }
        str[--(*position)] = '\0';
        printf("\b \b");
    }
}

void rel_path(char *path)
{
    char temp[1024] = "";
    int home_length, current_length;
    home_length = strlen(home_dir);
    current_length = strlen(path);
    if (home_length <= current_length)
    {
        strncpy(temp, path, home_length);
        if (strcmp(temp, home_dir) == 0)
        {
            temp[0] = '~';
            temp[1] = '\0';
            strcat(temp, path + home_length);
            strcpy(path, temp);
        }
    }
}

char *input_line()
{
    char temp_string[1024] = "", c;
    int i = 0, x = 0;
    his *node = NULL;
    setbuf(stdout, NULL);
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                if (read(STDIN_FILENO, buf, 2) == 2)
                {
                    if (buf[1] == 65 && buf[0] == 91)
                    {
                        while (x > 0)
                        {
                            backspace(temp_string, &x);
                        }
                        if (node == NULL)
                        {
                            node = his_start;
                        }
                        else if (node->prev != NULL)
                        {
                            node = node->prev;
                        }

                        if (node != NULL)
                        {
                            printf("%s", node->command);
                            x += strlen(node->command);
                            strcpy(temp_string, node->command);
                        }
                    }
                    if (buf[1] == 66 && buf[0] == 91)
                    {
                        while (x > 0)
                        {
                            backspace(temp_string, &x);
                        }
                        if (node != NULL)
                        {
                            if (node->next != NULL)
                            {
                                node = node->next;
                                printf("%s", node->command);
                                x += strlen(node->command);
                                strcpy(temp_string, node->command);
                            }
                        }
                        else
                        {
                            node = NULL;
                            strcpy(temp_string, "");
                        }
                    }
                }
            }
            else if (c == 127)
            {
                backspace(temp_string, &x);
                if (x == 0)
                {
                    node = NULL;
                }
            }
            else if (c == 9)
            { 
                temp_string[x++] = c;
                for (int i = 0; i < 8; i++)
                { 
                    printf(" ");
                }
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            temp_string[x++] = c;
        }
    }
    return strdup(temp_string);

    
}

int quitter(int sleeper)
{
    char c;
    int ret;
    int i = 1;
    clock_t current = clock();
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    setbuf(stdout, NULL);
    struct pollfd pfds;
    pfds.fd = STDIN_FILENO;
    pfds.events = POLLIN;
    while (1)
    {
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (c == 113)
            {
                break;
            }
        }
    }
    printf("\n");
    fcntl(STDIN_FILENO, F_SETFL, flags);
    return ret;
}