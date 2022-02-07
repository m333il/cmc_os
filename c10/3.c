#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

enum
{
    FIRST = 1,
    SECOND = 2,
    THIRD = 3,
};

int proc(char *str) {
    int status;
    pid_t pid = fork();
    if (!pid) {
        execlp(str, str, NULL);
        _exit(1);
    } else if (pid > 0) {
        wait(&status);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    return (!proc(argv[FIRST]) && !proc(argv[SECOND])) || !proc(argv[THIRD]);
}