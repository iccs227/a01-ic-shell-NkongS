#include "icsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

volatile sig_atomic_t fg_child_pid = 0;

int run_external(char *cmd, int *last_status, pid_t *child_pid) {
    char *argv_exec[MAX_CMD_BUFFER/2 + 2];
    int i = 0;
    char *infile = NULL, *outfile = NULL;

    // copy the co
    char cmd_copy[MAX_CMD_BUFFER];
    strncpy(cmd_copy, cmd, MAX_CMD_BUFFER);
    cmd_copy[MAX_CMD_BUFFER-1] = '\0';

    char *saveptr = NULL;
    char *token = strtok_r(cmd_copy, " ", &saveptr);

    while (token && i < MAX_CMD_BUFFER/2 + 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            if (token) infile = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            if (token) outfile = token;
        } else {
            argv_exec[i++] = token;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }
    argv_exec[i] = NULL;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        *last_status = 1;
        return 1;
    } else if (pid == 0) {
        // input redirect
        if (infile) {
            int fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror("open input");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        // output redirect
        if (outfile) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                perror("open output");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(argv_exec[0], argv_exec);
        fprintf(stderr, "icsh: command not found: %s\n", argv_exec[0]);
        exit(127);
    } else {
        *child_pid = pid;
        fg_child_pid = pid;
        int status;
        waitpid(pid, &status, 0);
        fg_child_pid = 0;
        if (WIFEXITED(status)) *last_status = WEXITSTATUS(status);
        else *last_status = 1;
        return 1;
    }
}
