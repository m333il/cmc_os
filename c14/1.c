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
    N_IND = 1,
    KEY_IND = 2,
    MAXVAL_IND = 3,
    DEC = 10,
    HEX = 16,
    PERMISSIONS = 0700,
};

void work(int nproc, int maxval, int curr_proc, int semid, int *shared_memory) {
    while (1) {
        struct sembuf buf;
        buf.sem_num = curr_proc;
        buf.sem_op = -1;
        buf.sem_flg = 0;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
        printf("%d %d %d\n", curr_proc, shared_memory[0], shared_memory[1]);
        fflush(stdout);
        ++shared_memory[0];
        if (shared_memory[0] > maxval) {
            _exit(0);
        }
        shared_memory[1] = curr_proc;

        int new_proc = shared_memory[0] % nproc;
        new_proc = ((new_proc % nproc) * (new_proc % nproc)) % nproc;
        new_proc = ((new_proc % nproc) * (new_proc % nproc)) % nproc + 1;
        buf.sem_num = new_proc;
        buf.sem_op = 1;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    int nproc = strtol(argv[N_IND], NULL, DEC);
    key_t key = strtol(argv[KEY_IND], NULL, HEX);
    int maxval = strtol(argv[MAXVAL_IND], NULL, DEC);

    int semid = semget(key, nproc + 1, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (semid < 0) {
        exit(1);
    }
    if (semctl(semid, 1, SETVAL, 1) < 0) {
        exit(1);
    }
    
    int *shared_memory = mmap(NULL, 2 * sizeof(*shared_memory), 
            PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (shared_memory == MAP_FAILED) {
        exit(1);
    }
    shared_memory[0] = 0;
    shared_memory[1] = 0;
    for (int i = 1; i <= nproc; ++i) {
        pid_t pid = fork();
        if (!pid) {
            work(nproc, maxval, i, semid, shared_memory);
        }
    }
    
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0)
        ;
    munmap(shared_memory, 2 * sizeof(*shared_memory));
    return 0;
}
