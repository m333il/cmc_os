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
    PERMISSIONS = 0660,
    WRONG = 42,
    COMMAND_IND = 1,
    FILE1_IND = 2,
    FILE2_IND = 3,
    FILE3_IND = 4,
};

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    int status;
    if (pid < 0) {
        exit(WRONG);
    } else if (!pid) {
        int fd = open(argv[FILE1_IND], O_RDONLY);
        if (fd < 0) {
            _exit(WRONG);
        }
        if (dup2(fd, 0) == -1) {
            _exit(WRONG);
        }
        if (close(fd) == -1) {
            _exit(WRONG);
        }   

        fd = open(argv[FILE2_IND], O_WRONLY | O_CREAT | O_APPEND, PERMISSIONS);
        if (fd < 0) {
            _exit(WRONG);
        }
        if (dup2(fd, 1) == -1) {
            _exit(WRONG);
        }
        if (close(fd) == -1) {
            _exit(WRONG);
        }   

        fd = open(argv[FILE3_IND], O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS); 
        if (fd < 0) {
            _exit(WRONG);
        }
        if (dup2(fd, 2) == -1) {
            _exit(WRONG);
        }
        if (close(fd) == -1) {
            _exit(WRONG);
        }  
        execlp(argv[COMMAND_IND], argv[COMMAND_IND], NULL);
        _exit(WRONG);
    } else {
        wait(&status);
        printf("%d\n", status);
    }

    return 0;
}
