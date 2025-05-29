#include "icsh.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void trim(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
}

int parse_exit_code(char *arg) {
    int code = atoi(arg);
    return code & 0xFF;
}