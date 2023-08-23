CC=gcc
CFLAGS=-I.
DEPS = header.h
OBJ = main.c linked_lists.c ls.c pinfo.c parse.c command_processing.c piping.c technical_functions.c shell_basics.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) 