#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

enum
{
    SIZE = 256,
};

int main(int argc, char *argv[]) {
    unsigned arr[SIZE] = {0};
    int amount = 0;
    unsigned max = 0;
    unsigned curr;
    while (scanf("%u", &curr) == 1) {
        if (curr > max) {
            max = curr;
        }
        arr[amount++] = curr;
    }
    for (int i = amount - 1; i >= 0; --i) {
        if (arr[i] && max % arr[i] == 0) {
            printf("%u\n", arr[i]);
        }
    }

    return 0;
}
