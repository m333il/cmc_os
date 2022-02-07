
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
    int fd[2];
    if (pipe(fd)){
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {
        close(fd[1]);
        wait(NULL);
        time_t t;
        if (read(fd[0], &t, sizeof(t)) < 0) {
            exit(1);
        }
        struct tm *date = localtime(&t);
        printf("Y:%04d\n", date->tm_year + 1900);
        fflush(stdout);
        close(fd[0]);
    } else if (!pid) {
        pid = fork();
        if (pid < 0) {
            _exit(1);
        } else if (pid > 0) {
            close(fd[1]);
            wait(NULL);
            time_t t;
            if (read(fd[0], &t, sizeof(t)) < 0) {
                _exit(1);
            }
            struct tm *date = localtime(&t);
            printf("M:%02d\n", date->tm_mon + 1);
            fflush(stdout);
            close(fd[0]);
        } else if (!pid) {
            pid = fork();
            if (pid < 0) {
                _exit(1);
            } else if (pid > 0) {
                close(fd[1]);
                wait(NULL);
                time_t t;
                if (read(fd[0], &t, sizeof(t)) < 0) {
                    _exit(1);
                }
                struct tm *date = localtime(&t);
                printf("D:%02d\n", date->tm_mday);
                fflush(stdout);
                close(fd[0]);
            } else if (!pid) {
                close(fd[0]);
                time_t t = time(NULL);
                for (int j = 0; j < 3; j++) {
                    if(write(fd[1], &t, sizeof(t)) < 0) {
                        _exit(1);
                    }
                }
                close(fd[1]);
            }
        }
    }
    return 0;
}
