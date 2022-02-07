#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    AMOUNT_OF_SONS = 3,
    BUF_SIZE = 8,
    BASE = 10,
};

int main(void) {
    for (int i = 0; i < AMOUNT_OF_SONS; ++i) {
        int pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            char curr[BUF_SIZE] = "";
            read(0, curr, sizeof(curr));
            int num = strtol(curr, NULL, BASE);
            printf("%d %d\n", i + 1, num * num);
            fflush(stdout);
            _exit(0);
        }
    }
    while (wait(NULL) > 0)
        ;

    return 0;
}