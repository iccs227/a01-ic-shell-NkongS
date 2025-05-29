#include "jobs.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

job_t jobs[MAX_JOBS];
int next_job_id = 1;

void add_job(pid_t pid, const char *cmd) {
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobs[i].pid == 0) {
            jobs[i].job_id = next_job_id++;
            jobs[i].pid = pid;
            strncpy(jobs[i].cmd, cmd, MAX_CMD_BUFFER-1);
            jobs[i].cmd[MAX_CMD_BUFFER-1] = '\0';
            strcpy(jobs[i].status, "Running");
            break;
        }
    }
}

void remove_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobs[i].pid == pid) {
            jobs[i].pid = 0;
            jobs[i].cmd[0] = '\0';
            jobs[i].status[0] = '\0';
            jobs[i].job_id = 0;
            break;
        }
    }
}

void print_jobs() {
    for (int id = 1; id < next_job_id; ++id) {
        for (int i = 0; i < MAX_JOBS; ++i) {
            if (jobs[i].pid != 0 && jobs[i].job_id == id) {
                printf("[%d]  %-8s                 %s &\n",
                    jobs[i].job_id, jobs[i].status, jobs[i].cmd);
                break;
            }
        }
    }
}

job_t* find_job_by_id(int job_id) {
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobs[i].job_id == job_id) return &jobs[i];
    }
    return NULL;
}

job_t* find_job_by_pid(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; ++i) {
        if (jobs[i].pid == pid) return &jobs[i];
    }
    return NULL;
}