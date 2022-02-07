#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>

void handler(int s){
    static int count = 0;
    if (count == 5) {
        exit(0);
    }
    printf("%d\n", ++);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}
