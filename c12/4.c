#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    BASE = 10,
    AMOUNT_OF_SONS = 2,
};

volatile sig_atomic_t flag = 0;

void handler(int s) {
    flag = 1;
}

int main(int argc, char *argv[]) {
    int n = strtol(argv[1], NULL, BASE);
    if (n == 1) {
        printf("Done\n");
        fflush(stdout);
        return 0;
    }

    int fd[2];
    if (pipe(fd) < 0){
        exit(1);
    }

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sig, NULL);
    sigemptyset(&sig);

    FILE *fin = fdopen(fd[0], "r");
            FILE *fout = fdopen(fd[1], "w");
    
    pid_t sons[AMOUNT_OF_SONS] = {0};
    for (int i = 0; i < AMOUNT_OF_SONS; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (pid) {
            sons[i] = pid;
        } else if (!pid) {
            int x = 1;
            while (1) {
                while (!flag) {
                    sigsuspend(&sig);
                }
                flag = 0;
                pid_t brother_pid;
                fscanf(fin, "%d %d", &brother_pid, &x);

                if (x == n) {
                    fprintf(fout, "%d %d ", getpid(), x);
                    fflush(fout);
                    kill(brother_pid, SIGUSR1);
                    fclose(fin);
                    fclose(fout);
                    _exit(0);
                }
                
                printf("%d %d\n", i + 1, x++);
                fflush(stdout);
                
                fprintf(fout, "%d %d ", getpid(), x);
                fflush(fout);
                kill(brother_pid, SIGUSR1);
            }
        }
    }
    fprintf(fout, "%d 1 ", sons[1]);
    fflush(fout);
    fclose(fout);
    fclose(fin);
    kill(sons[0], SIGUSR1);
    while (wait(NULL) > 0)
        ;
    
    printf("Done\n");
    fflush(stdout);
    return 0;
}