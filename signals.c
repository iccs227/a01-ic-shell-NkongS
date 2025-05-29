#include "icsh.h"
#include "jobs.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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

void sigchld_handler(int sig) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        job_t *job = find_job_by_pid(pid);
        if (job) {
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                printf("\n[%d] Done %s\nicsh $ ", job->job_id, job->cmd);
                fflush(stdout);
                remove_job(pid);
            } else if (WIFSTOPPED(status)) {
                strcpy(job->status, "Stopped");
                printf("\n[%d]+  Stopped %s\nicsh $ ", job->job_id, job->cmd);
                fflush(stdout);
            } else if (WIFCONTINUED(status)) {
                strcpy(job->status, "Running");
            }
        }
    }
}

void setup_signal_handlers() {
    struct sigaction sa_int, sa_tstp, sa_chld;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_tstp, NULL);

    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa_chld, NULL);
}