#include "icsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t fg_child_pid = 0;

int run_external(char *cmd, int *last_status, pid_t *child_pid) {
    char *argv_exec[MAX_CMD_BUFFER/2 + 2];
    int i = 0;
    char *saveptr;
    char *token = strtok_r(cmd, " ", &saveptr);
    while (token && i < MAX_CMD_BUFFER/2 + 1) {
        argv_exec[i++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }
    argv_exec[i] = NULL;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        *last_status = 1;
        return 1;
    } else if (pid == 0) {
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
void reset_child_pid() { fg_child_pid = 0; }