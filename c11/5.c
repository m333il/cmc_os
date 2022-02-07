#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    BASE = 10,
};

void print_numbers(FILE *print, FILE *scan, int n, int i) {
    int a = 1;
    while (fscanf(scan, "%d", &a) > 0 && a < n) {
        printf("%d %d\n", i, a++);
        if (fflush(stdout) == EOF) {
            _exit(1);
        }
        fprintf(print, "%d ", a);
        if (fflush(print) == EOF) {
            _exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    int n = strtol(argv[1], NULL, BASE);
    int fd1[2];
    if (pipe(fd1) < 0) {
        exit(1);
    }
    int fd2[2];
    if (pipe(fd2) < 0) {
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        close(fd2[1]);
        close(fd1[0]);
        FILE *print = fdopen(fd1[1], "w");
        FILE *scan = fdopen(fd2[0], "r");
        if (print == NULL || scan == NULL) {
            _exit(1);
        }

        print_numbers(print, scan, n, 1);
        fclose(scan);
        fclose(print);
        _exit(0);
    } else if (pid > 0) {
        pid = fork();
        if (pid < 0){
            exit(1);
        } else if (!pid) {
            close(fd1[1]);
            close(fd2[0]);
            
            FILE *scan = fdopen(fd1[0], "r");
            FILE *print = fdopen(fd2[1], "w");
            if (print == NULL || scan == NULL) {
                _exit(1);
            }
            
            print_numbers(print, scan, n, 2);
            fclose(scan);
            fclose(print);
            _exit(0);
        } else if (pid > 0) {
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            
            FILE *print = fdopen(fd2[1], "w");
            if (print == NULL) {
                exit(1);
            }
            fprintf(print, "%d ", 1);
            fclose(print);
            wait(NULL);
            wait(NULL);
            printf("Done\n");
        }
    }
    return 0;
}
