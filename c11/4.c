#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

enum
{
    STDIN = 0,
    STDOUT = 1,
};

void error(pid_t *pids, int l) {
    for(int i = 0; i < l; i++) {
        if (kill(pids[i], SIGKILL) < 0) {
            _exit(1);
        }
    }
    while(wait(NULL) > 0)
        ;
    free(pids);
    exit(1);
}

int main(int argc, char *argv[]) {
    pid_t *sons = calloc(argc - 1, sizeof(*sons));
    if (!sons) {
        exit(1);
    }
    int fd[2];
    int read_des = -1;
    
    for (int i = 1; i < argc; i++) {
        pipe(fd);
        pid_t pid = fork();
        if (pid < 0) {
            error(sons, i - 1);
        } else if (pid > 0) {
            //parent
            sons[i - 1] = pid;
            if (i > 1) {
                close(read_des) < 0;
            }
            if (i != argc - 1) {
                read_des = fd[0];
            } else {
                close(fd[0]) < 0;
            }
            close(fd[1]) < 0;
        } else if (!pid) {  
            //son
            close(fd[0]) < 0;
            if (i > 1) {
                dup2(read_des, STDIN);
                close(read_des);
            }
            if (i != argc - 1) {
                dup2(fd[1], STDOUT)
            }
            close(fd[1]);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        
    }
    free(sons);
    while(wait(NULL) > 0)
        ;
    return 0;
}
