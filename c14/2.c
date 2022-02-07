#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>

enum 
{
    BASE = 10,
    PERMISSIONS = 0600,
    MOD = 10,
    COUNT_IND = 1,
    KEY_IND = 2,
    NPROC_IND = 3,
    ITER_COUNT_IND = 4,
    SEED_IND = 5,
    AMOUNT_OF_OPERATIONS = 2,
};

void work(int semid, int iter_count, int count, int seed, int *arr) {
    srand(seed);
    for (int i = 0; i < iter_count; ++i) {
        int rand_ind1 = rand() % count; 
        int rand_ind2 = rand() % count;
        int rand_num = rand() % MOD;
        
        struct sembuf down1 = { .sem_num = rand_ind1, .sem_op = -1, .sem_flg = 0 };
        struct sembuf down2 = { .sem_num = rand_ind2, .sem_op = -1, .sem_flg = 0 };
        struct sembuf down[AMOUNT_OF_OPERATIONS] =  { down1, down2 };

        if (rand_ind1 != rand_ind2) {
            semop(semid, down, AMOUNT_OF_OPERATIONS);
        } else {
            semop(semid, down, AMOUNT_OF_OPERATIONS - 1);
        }
        operation(arr, rand_ind1, rand_ind2, rand_num);

        struct sembuf up1 = { .sem_num = rand_ind1, .sem_op = 1, .sem_flg = 0 };
        struct sembuf up2 = { .sem_num = rand_ind2, .sem_op = 1, .sem_flg = 0 };
        struct sembuf up[AMOUNT_OF_OPERATIONS] =  { up1, up2 };

        if (rand_ind1 != rand_ind2) {
            semop(semid, up, AMOUNT_OF_OPERATIONS);
        } else {
            semop(semid, up, AMOUNT_OF_OPERATIONS - 1);
        }
    }
    _exit(0);
}

int main(int argc, char *argv[]) {
    int count = strtol(argv[COUNT_IND], NULL, BASE);
    key_t key = strtol(argv[KEY_IND], NULL, BASE);
    int nproc = strtol(argv[NPROC_IND], NULL, BASE);
    int iter_count = strtol(argv[ITER_COUNT_IND], NULL, BASE);
    
    int *arr = mmap(NULL, count * sizeof(*arr), PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (arr == MAP_FAILED) {
        exit(1);
    }

    for (int i = 0; i < count; ++i) {
        scanf("%d", &arr[i]);
    }
    
    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (semid < 0) {
        exit(1);
    }
    for (int i = 0; i < count; ++i) {
        semctl(semid, i, SETVAL, 1);
    }
    
    for (int i = 0; i < nproc; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            munmap(arr, count * sizeof(*arr));
            exit(1);
        } else if (!pid) {
            int seed = strtol(argv[SEED_IND + i], NULL, BASE);
            work(semid, iter_count, count, seed, arr);
        }
    }
    
    while (wait(NULL) > 0)
        ;
    for (int i = 0; i < count; ++i) {
        printf("%d\n", arr[i]);
    }
    semctl(semid, 0, IPC_RMID);
    munmap(arr, count * sizeof(*arr));
    return 0;
}