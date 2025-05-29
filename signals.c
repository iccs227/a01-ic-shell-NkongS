#include "icsh.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t fg_child_pid = 0;

void sigint_handler(int sig) {
    if (fg_child_pid > 0) {
        kill(fg_child_pid, SIGINT);
    }
}

void sigtstp_handler(int sig) {
    if (fg_child_pid > 0) {
        kill(fg_child_pid, SIGTSTP);
    }
}

void setup_signal_handlers() {
    struct sigaction sa_int, sa_tstp;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_tstp, NULL);
}