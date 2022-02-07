#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd)) {
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {
        int curr;
        close(fd[0]);
        while (scanf("%d", &curr) > 0) {
            write(fd[1], &curr, sizeof(curr));
        }
        close(fd[1]);
        wait(NULL);
    } else if (!pid) {
        pid = fork();
        if (pid < 0){
            _exit(1);
        } else if (pid > 0) {
            close(fd[1]);
            close(fd[0]);
            wait(NULL);
            exit(0);
        } else if (!pid) {
            close(fd[1]);
            long long sum = 0;
            int curr;
            while (read(fd[0], &curr, sizeof(curr)) > 0) {
                sum += curr;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            close(fd[0]);
            exit(0);
        }
    }
    return 0;
}