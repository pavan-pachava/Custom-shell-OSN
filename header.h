#ifndef HEADERS_H
#define HEADERS_H

// header files

#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <termios.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// shell_basics.c Functions

void shell_setup();

void shell_prompt();

void shell();

void child_terminate(int signal);

void shell_close(int signal);

void fg_to_bg_handler(int signal);

void ctrl_C_handler(int signal);

// command_processing.c Functions

void command_process(char command[]);

void cd_com(char **argument);

void echo_com(char **argument, int parts);

void pwd_com();

void bg_com(char **argument, int parts);

void fg_com(char **argument, int parts);

void make_fg_com(int proc);

void make_bg_com(int proc);

// technical_functions.c Functions

void remove_spaces(char string[]);

char *combination(char **argument, int start_part, int end_part);

void backspace(char temp_string[], int *position);

char *input_line();

int quitter(int sleep_period);

void rel_path(char *path);

// parse.c Functions

char **parse(char string[], char delimiter[], int *parts);

void free_memory(char ***mem, int no);

// pinfo.c Functions

void pinfo_com(int pid);

int search_state(int pid);

// ls.c Functions

void ls_command(int a_flag, int l_flag, int mul_flag, char *dir);

void list_form(char *path, char *name, int df);

// linked_list.c Functions

typedef struct Background_Processes
{
    int pid;
    int pos;
    char *process_name;
    struct Background_Processes *next;
    struct Background_Processes *prev;
} bgp;

typedef struct History
{
    char *command;
    struct History *next;
    struct History *prev;
} his;

void add_bgp_node(int pid, char *process_name);

char *get_process_name(int pid);

void remove_process(int pid);

void add_his_node(char *command);

void history_command(his *node, int no);

void history_storage();

void history_retrieval();

// piping.c Functions

void bripe(char* command);

// Global Variables

extern char username[1024];
extern char systemname[1024];
extern char home_dir[1024];
extern char prev_dir[1024];
extern bgp *bgp_start;
extern bgp *bgp_end;
extern his *his_start;
extern his *his_end;
extern int his_count;
extern int bgp_count;
extern int fg_proc;
extern char* fg_proc_name;
extern char colorCode;
extern int parent;
#endif