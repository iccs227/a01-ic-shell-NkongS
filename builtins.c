#include "icsh.h"
#include "jobs.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int handle_builtin(char *cmd, int *last_status) {
    char *argv[MAX_CMD_BUFFER/2 + 2];
    int argc = 0;
    char *output_file = NULL;

    // tokenize the command
    char *saveptr = NULL;
    char *token = strtok_r(cmd, " ", &saveptr);
    while (token) {
        if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            if (token) output_file = token;
            break;
        }
        argv[argc++] = token;
        token = strtok_r(NULL, " ", &saveptr);
    }
    argv[argc] = NULL;

    int out_fd = -1, saved_stdout = -1;
    if (output_file) {
        out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (out_fd < 0) {
            perror("open output");
            return 1;
        }
        saved_stdout = dup(STDOUT_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    if (argc > 0 && strcmp(argv[0], "echo") == 0) {
        if (argc > 1 && strcmp(argv[1], "$?") == 0) {
            printf("%d\n", *last_status);
        } else if (argc > 1) {
            for (int i = 1; i < argc; ++i) {
                printf("%s", argv[i]);
                if (i < argc - 1) printf(" ");
            }
            printf("\n");
        } else {
            printf("\n");
        }
        *last_status = 0;
        if (saved_stdout != -1) {
            fflush(stdout);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        return 1;
    } else if (argc > 0 && strcmp(argv[0], "exit") == 0) {
        int code = 0;
        if (argc > 1) code = parse_exit_code(argv[1]);
        printf("bye\n");
        exit(code);
    }
    if (saved_stdout != -1) {
        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }

    if (argc > 1 && strcmp(argv[0], "fg") == 0) {
        int job_id = atoi(argv[1] + 1);
        job_t *job = find_job_by_id(job_id);
        if (job && job->pid != 0) {
            tcsetpgrp(STDIN_FILENO, job->pid);
            kill(-job->pid, SIGCONT);
            int status;
            waitpid(job->pid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpid());
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                remove_job(job->pid);
            } else if (WIFSTOPPED(status)) {
                job_t *fgjob = find_job_by_pid(job->pid);
                if (!fgjob) {
                    add_job(job->pid, job->cmd);
                    fgjob = find_job_by_pid(job->pid);
                }
                if (fgjob) {
                    strcpy(fgjob->status, "Stopped");
                    printf("\n[%d]+  Stopped %s\nicsh $ ", fgjob->job_id, fgjob->cmd);
                    fflush(stdout);
                }
            }
        }
        *last_status = 0;
        return 1;
    }
    if (argc > 1 && strcmp(argv[0], "bg") == 0) {
        int job_id = atoi(argv[1] + 1); 
        job_t *job = find_job_by_id(job_id);
        if (job && job->pid != 0) {
            kill(-job->pid, SIGCONT);
            strcpy(job->status, "Running");
            printf("[%d]+ %s &\n", job->job_id, job->cmd);
            fflush(stdout);
        }
        *last_status = 0;
        return 1;
    }
    if (argc > 0 && strcmp(argv[0], "jobs") == 0) {
        print_jobs();
        *last_status = 0;
        return 1;
    }

    if (argc > 0 && strcmp(argv[0], "game") == 0) {
        if (argc == 2 && strcmp(argv[1], "ls") == 0) {
            printf("Available games:\n- snake\n");
            fflush(stdout);
            *last_status = 0;
            return 1;
        } else if (argc == 2 && strcmp(argv[1], "snake") == 0) {
            play_snake();
            *last_status = 0;
            return 1;
        } else {
            printf("Usage: game ls | game [game_name]\n");
            fflush(stdout);
            *last_status = 1;
            return 1;
        }
    }
    return 0;
}