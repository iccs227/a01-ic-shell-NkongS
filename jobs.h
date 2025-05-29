#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_CMD_BUFFER 255
#define MAX_JOBS 100

typedef struct job {
    int job_id;
    pid_t pid;
    char cmd[MAX_CMD_BUFFER];
    char status[16];
} job_t;

job_t* find_job_by_id(int job_id);
job_t* find_job_by_pid(pid_t pid);
void add_job(pid_t pid, const char *cmd);
void remove_job(pid_t pid);
void print_jobs();
extern int next_job_id;

#endif