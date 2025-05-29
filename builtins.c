#include "icsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_builtin(char *cmd, int *last_status) {
    char *saveptr;
    char *token = strtok_r(cmd, " ", &saveptr);
    if (!token) return 0;

    if (strcmp(token, "echo") == 0) {
        char *arg = strtok_r(NULL, "", &saveptr);
        if (arg && strcmp(arg, "$?") == 0) {
            printf("%d\n", *last_status);
        } else if (arg) {
            printf("%s\n", arg);
        } else {
            printf("\n");
        }
        *last_status = 0;
        return 1;
    } else if (strcmp(token, "exit") == 0) {
        char *arg = strtok_r(NULL, " ", &saveptr);
        int code = 0;
        if (arg) code = parse_exit_code(arg);
        printf("bye\n");
        exit(code);
    }
    return 0;
}