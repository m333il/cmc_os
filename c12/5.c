#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>

enum
{
    EXIT = 4,
};

volatile int last_prime = 0;
volatile int flag = 0;

void handler(int s){
    static int count = 1;
    if (s == SIGTERM) {
        exit(0);
    }
    if (count++ == EXIT) {
        exit(0);
    }
    flag = 1;
}

int is_prime(int x) {
    x = x > 0 ? x : -x;
    if (x == 1 || x % 2 == 0) {
        return 0;
    }
    unsigned t = x;
    for (unsigned i = 3; i * i <= t; i += 2) {
        if (x % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    int low, high;
    scanf("%d %d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    for (int i = low; i < high; ++i) {
        if (flag) {
            printf("%d\n", last_prime);
            fflush(stdout);
            flag = 0;
        }
        if (is_prime(i)) {
            last_prime = i;
        }
    }
    printf("-1\n");
    return 0;
}