#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

enum
{
    PERMISSIONS = 0660,
    CMD1_IND = 1,
    CMD2_IND = 2,
    CMD3_IND = 3,
    FILE1_IND = 4,
    FILE2_IND = 5,
    STDIN = 0,
    STDOUT = 1,
};

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0){
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid < 0){
        exit(1);
    } else if (!pid) {
        if (close(fd[0]) < 0) {
            _exit(1);
        }
        if (dup2(fd[1], STDOUT) < 0){
            _exit(1);
        }
        if (close(fd[1]) < 0) {
            _exit(1);
        }
        pid = fork();
        if (pid < 0){
            _exit(1);
        } else if (!pid) {
            int filedes = open(argv[FILE1_IND], O_RDONLY);
            if (filedes < 0) {
                _exit(1);
            }
            if (dup2(filedes, STDIN) < 0){
                _exit(1);
            }
            if (close(filedes) < 0) {
                _exit(1);
            }
            execlp(argv[CMD1_IND], argv[CMD1_IND], NULL);
            _exit(1);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                execlp(argv[CMD2_IND], argv[CMD2_IND], NULL);
                _exit(1);
            }
            _exit(0);
        }
    }
    if (close(fd[1]) < 0) {
        exit(1);
    }
    pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        int filedes = open(argv[FILE2_IND], O_WRONLY | O_CREAT | O_APPEND, PERMISSIONS);
        if (filedes < 0) {
            _exit(1);
        }
        if (dup2(fd[0], STDIN) < 0) {
            _exit(1);
        }
        if (close(fd[0]) < 0) {
            _exit(1);
        }
        if (dup2(filedes, STDOUT) < 0) {
            _exit(1);
        }
        if (close(filedes) < 0) {
            _exit(1);
        }
        execlp(argv[CMD3_IND], argv[CMD3_IND], NULL);
        _exit(1);
    }
    if (close(fd[0]) < 0) {
        _exit(1);
    }
    while (wait(NULL) > 0)
        ;
    return 0;
}
