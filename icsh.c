/* ICCS227: Project 1: icsh
 * Name: Naruebet Songsri
 * StudentID: 6580006
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CMD_BUFFER 255

// trim whitespace
void trim(char *str) {
    char *end;
    // leading
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    // trailing
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
}

// parse exit code helper
int parse_exit_code(char *arg) {
    int code = atoi(arg);
    return code & 0xFF;
}

int main(int argc, char *argv[]) {
    char buffer[MAX_CMD_BUFFER];
    char last_cmd[MAX_CMD_BUFFER] = "";
    FILE *input = stdin;

    // open file for reading
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "icsh: cannot open file %s\n", argv[1]);
            return 1;
        }
    }

    if (input == stdin)
        printf("Starting IC shell\n");

    while (1) {
        if (input == stdin)
            printf("icsh $ ");

        // read command from file
        if (!fgets(buffer, MAX_CMD_BUFFER, input)) {
            if (input != stdin) fclose(input);
            if (input == stdin) printf("\n");
            break;
        }

        // rm trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        trim(buffer);

        if (strlen(buffer) == 0) continue;

        // handle !!
        if (strcmp(buffer, "!!") == 0) {
            if (strlen(last_cmd) == 0) continue;
            printf("%s\n", last_cmd);
            strcpy(buffer, last_cmd);
        } else {
            strcpy(last_cmd, buffer);
        }

        // parse command
        char *cmd = strtok(buffer, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "echo") == 0) {
            char *arg = strtok(NULL, "");
            if (arg) printf("%s\n", arg);
            else printf("\n");
        } else if (strcmp(cmd, "exit") == 0) {
            char *arg = strtok(NULL, " ");
            int code = 0;
            if (arg) code = parse_exit_code(arg);
            printf("bye\n");
            exit(code);
        } else {
            printf("bad command\n");
        }
    }
    return 0;
}
