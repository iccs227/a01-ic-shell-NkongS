#ifndef ICSH_H
#define ICSH_H

#include <sys/types.h>
#include <signal.h>

#define MAX_CMD_BUFFER 255

void trim(char *str);
int parse_exit_code(char *arg);
int handle_builtin(char *cmd, int *last_status);
int run_external(char *cmd, int *last_status, pid_t *child_pid);
void setup_signal_handlers();
void reset_child_pid();

extern volatile sig_atomic_t fg_child_pid;

#endif