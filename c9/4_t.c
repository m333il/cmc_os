#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    SON1 = 1,
    SON2 = 2,
    SON3 = 3,
    BUF_SIZE = 8,
    BASE = 10,
};

int main(void) {

    int pid1 = 1, pid2 = 1, pid3 = 3;
    int parent = getpid();
    if (getpid() == parent) {
        pid1 = fork();
    }
    if (getpid() == parent) {
        pid2 = fork();
    }
    if (getpid() == parent) {
        pid3 = fork();
    }
    
    if (!pid1 || !pid2 || !pid3) {
        char buf[BUF_SIZE];
        read(0, buf, sizeof(buf));
        buf[strlen(buf) - 1] = '\0';
        int num = strtol(buf, NULL, BASE);
        num *= num;
        if (!pid1) {
            printf("%d %d\n", SON1, num);
        } else if (!pid2) {
            printf("%d %d\n", SON2, num);
        } else if (!pid3) {
            printf("%d %d\n", SON3, num);
        }
        fflush(stdout);
        _exit(0);
    } else {
        while (wait(NULL) > 0) {}
    }

    return 0;
}