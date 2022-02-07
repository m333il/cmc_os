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
    PERMISSIONS = 0600,
    BASE = 10,
    KEY = 333,
    N_IND = 1,
    L_IND = 2,
};

typedef struct MsgBuf
{
    long mtype;
    unsigned value[1];
} MsgBuf;

void work(int msgid, int l, unsigned curr_proc, unsigned n, int *mem) {
    while (1) {
        MsgBuf m;
        if (msgrcv(msgid, &m, sizeof(m) - sizeof(m.mtype), curr_proc + 1, 0) < 0) {
            _exit(0);
        }
        if (--l >= 0) {
            printf("%d %d\n", curr_proc, l);
        } else {
            MsgBuf c = { .mtype = n + 1, .value = {1}};
            msgsnd(msgid, &c, sizeof(c) - sizeof(c.mtype), 0);
            ++mem[0];
            _exit(1);
        }
        fflush(stdout);
    }
}

int main(int argc, char *argv[]) {
    unsigned n = strtoul(argv[N_IND], NULL, BASE);
    int l = strtol(argv[L_IND], NULL, BASE);
    
    int msgid = msgget(KEY, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (msgid < 0) {
        exit(1);
    }

    int *shared_memory = mmap(NULL, 1 * sizeof(*shared_memory), 
            PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    shared_memory[0] = 0;

    for (unsigned i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            exit(1);
        } else if (!pid) {
            work(msgid, l, i, n, shared_memory);
        }
    }
    unsigned curr;
    while (scanf("%u", &curr) == 1) {
        MsgBuf m = { .mtype = curr % n + 1, .value = {1}};
        msgsnd(msgid, &m, sizeof(m) - sizeof(m.mtype), 0);
        msgrcv(msgid, &m, sizeof(m) - sizeof(m.mtype), n + 1, 0);
    } 

    while (wait(NULL) > 0)
        ;
    printf("%u\n", n - shared_memory[0]);
    msgctl(msgid, IPC_RMID, NULL);
    while (wait(NULL) > 0)
        ;
    munmap(shared_memory, sizeof(*shared_memory));
    return 0;
}
