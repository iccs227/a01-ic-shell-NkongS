CC=gcc
CFLAGS=-Wall -g 
BINARY=icsh

all: icsh

icsh: icsh.c builtins.c external.c signals.c utils.c jobs.c snake.c
	$(CC) -o $(BINARY) $(CFLAGS) $^

.PHONY: clean

clean:
	rm -f $(BINARY)
