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
#include <sys/msg.h>


enum
{
    PERMISSIONS = 0600,
    BASE = 10,
    KEY = 1234567,
    N_IND = 1,
    L_IND = 2,
};

typedef struct MsgBuf
{
    long mtype;
    unsigned value[1];
} MsgBuf;

void work(int semid, int l, unsigned curr_proc, int *mem, int nproc) {
    while (1) {
        struct sembuf buf;
        buf.sem_num = curr_proc;
        buf.sem_op = -1;
        buf.sem_flg = 0;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
        if (--l >= 0) {
            printf("%d %d\n", curr_proc, l);
            fflush(stdout);
        }
        if (l == 0) {
            ++mem[0];
        }
        int new_proc = nproc;
        buf.sem_num = new_proc;
        buf.sem_op = 1;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    unsigned n = strtoul(argv[N_IND], NULL, BASE);
    int l = strtol(argv[L_IND], NULL, BASE);
    

    
    int semid = semget(KEY, n + 1, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (semid < 0) {
        exit(1);
    }

    int *shared_memory = mmap(NULL, 1 * sizeof(*shared_memory), 
            PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    shared_memory[0] = 0;

    for (unsigned i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            exit(1);
        } else if (!pid) {
            work(semid, l, i, shared_memory, n);
        }
    }
    unsigned curr;
    while (scanf("%u", &curr) == 1) {
        if (semctl(semid, curr % n, SETVAL, 1) < 0) {
            exit(1);
        }
        struct sembuf buf;
        buf.sem_num = n;
        buf.sem_op = -1;
        buf.sem_flg = 0;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
    } 
    printf("%u\n", n - shared_memory[0]);
    semctl(semid, 0, IPC_RMID);
    signal(SIGINT, SIG_IGN);
    kill(0, SIGINT);
    while (wait(NULL) > 0)
        ;
    return 0;
}
