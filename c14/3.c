#include <inttypes.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

enum
{
    PERMISSIONS = 0600,
    BASE = 10,
    KEY_IND = 1,
    N_IND = 2,
    VAL1_IND = 3,
    VAL2_IND = 4,
    MAXVAL_IND = 5,
};

typedef struct MsgBuf
{
    long mtype;
    int64_t value[2];
} MsgBuf;

void work(int msgid, int64_t maxval, int curr_proc, int n) {
    while (1) {
        MsgBuf m;
        if (msgrcv(msgid, &m, sizeof(m) - sizeof(m.mtype), curr_proc + 1, 0) < 0) {
            _exit(0);
        }

        printf("%d %" PRId64 "\n", curr_proc, m.value[0] + m.value[1]);
        fflush(stdout);
        
        int64_t value3 = m.value[0] + m.value[1];
        if (value3 > maxval) {
            _exit(0);
        }
        m.value[0] = m.value[1];
        m.value[1] = value3;
        m.mtype = (value3 % n) + 1;
        
        if (msgsnd(msgid, &m, sizeof(m) - sizeof(m.mtype), 0) < 0) {
            _exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    key_t key = strtol(argv[KEY_IND], NULL, BASE);
    int n = strtol(argv[N_IND], NULL, BASE);
    int64_t value1, value2, maxval;
    sscanf(argv[VAL1_IND], "%" SCNd64, &value1);
    sscanf(argv[VAL2_IND], "%" SCNd64, &value2);
    sscanf(argv[MAXVAL_IND], "%" SCNd64, &maxval);
    
    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (msgid < 0) {
        exit(1);
    }

    pid_t *pids = calloc(n, sizeof(*pids));
    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            for (int j = 0; j < n && pids[j] > 0; ++j) {
                kill(pids[j], SIGKILL);
            }
            while (wait(NULL) > 0)
                ;
            free(pids);
            exit(1);
        } else if (pid > 0) {
            pids[i] = pid;
        } else if (!pid) {
            work(msgid, maxval, i, n);
        }
    }
    free(pids);
    MsgBuf m = { .mtype = 1, .value = { value1, value2 }};
    msgsnd(msgid, &m, sizeof(m) - sizeof(m.mtype), 0);

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
    while (wait(NULL) > 0)
        ;
    return 0;
}