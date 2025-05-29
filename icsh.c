#include "icsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    char buffer[MAX_CMD_BUFFER];
    char last_cmd[MAX_CMD_BUFFER] = "";
    int last_status = 0;
    FILE *input = stdin;
    pid_t child_pid = 0;

    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "icsh: cannot open file %s\n", argv[1]);
            return 1;
        }
    }

    setup_signal_handlers();

    if (input == stdin)
        printf("Starting IC shell\n");

    while (1) {
        if (input == stdin)
            printf("icsh $ ");

        if (!fgets(buffer, MAX_CMD_BUFFER, input)) {
            if (input != stdin) fclose(input);
            if (input == stdin) printf("\n");
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        trim(buffer);

        if (strlen(buffer) == 0) continue;

        if (strcmp(buffer, "!!") == 0) {
            if (strlen(last_cmd) == 0) continue;
            printf("%s\n", last_cmd);
            strcpy(buffer, last_cmd);
        } else {
            strcpy(last_cmd, buffer);
        }

        // try builtin
        if (handle_builtin(buffer, &last_status)) continue;

        // try external
        run_external(buffer, &last_status, &child_pid);
    }
    return 0;
}