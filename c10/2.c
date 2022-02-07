#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

enum
{
    RET = 128,
};

int mysys(const char *str) {
    int status = 0;
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (!pid) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + RET;
        }   
        if (WIFEXITED(status) && WEXITSTATUS(status) >= 0 && WEXITSTATUS(status) <= 127) {
            return WEXITSTATUS(status);
        }
        return 0;
    }
}