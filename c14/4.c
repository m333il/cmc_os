#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


enum 
{
    COUNT_IND = 1,
    BASE = 10,
    PERMISSIONS = 0600,
    KEY_DEFAULT = 333333,
};

void work(int semid, int count, int curr_proc) {
    while (1) {
        struct sembuf buf;
        buf.sem_num = curr_proc;
        buf.sem_op = -1;
        buf.sem_flg = 0;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
        
        int num;
        if (scanf("%d", &num) != 1) {
            _exit(0);
        }
        printf("%d %d\n", curr_proc, num);
        fflush(stdout);
        int new_proc = (num % count + count) % count;
        buf.sem_num = new_proc;
        buf.sem_op = 1;
        if (semop(semid, &buf, 1) < 0) {
            _exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    setbuf(stdin, NULL);
    int count = strtol(argv[COUNT_IND], NULL, BASE);
    key_t key = KEY_DEFAULT;
    int rand = open("/dev/urandom", O_RDONLY);
    if (rand != -1) {
        read(rand, &key, sizeof(key));
    }

    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (semid < 0) {
        exit(1);
    }
    
    for (int i = 0; i < count; ++i) {
        pid_t pid = fork();
        if (!pid) {
            work(semid, count, i);
        }
    }
    semctl(semid, 0, SETVAL, 1);
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0)
        ;
    return 0;
}