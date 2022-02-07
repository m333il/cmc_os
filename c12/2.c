#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>

enum
{
    ADDING = 0,
    MULTIPLICATION = 1,
};

volatile int mode = ADDING;

void handler(int s){
    if (s == SIGINT) {
        mode = ADDING;
    } else if (s == SIGQUIT) {
        mode = MULTIPLICATION;
    }
}

int main(int argc, char *argv[]) {
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int res = 0;
    int curr;
    while (scanf("%d", &curr) > 0) {
        if (mode == ADDING) {
            res = (unsigned) res + curr;
        } else if (mode == MULTIPLICATION) {
            res = (unsigned) res * curr;
        }
        printf("%d\n", res);
        fflush(stdout);
    }
    return 0;
}