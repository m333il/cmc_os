#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    BASE = 10,
    N_IND = 1,
    FILE_START_IND = 2,
};

int wait_sons() {
    int amount = 0;
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)){
            ++amount;
        }
    }
    return amount;
}

void work(char *name) {
    FILE *file = fopen(name, "r");
    if (file == NULL){
        _exit(1);
    }
    char cmd[PATH_MAX];
    fgets(cmd, PATH_MAX, file);
    fclose(file);
    cmd[strlen(cmd) - 1] = '\0';
    execlp(cmd, cmd, NULL);
    _exit(1);
}

int main(int argc, char *argv[]) {
    int amount = 0;
    int n = strtol(argv[N_IND], NULL, BASE);
    int i;
    for (i = FILE_START_IND; i < argc && i < n + FILE_START_IND; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            work(argv[i]);
        }
    }

    amount += wait_sons();
    
    for (; i < argc; ++i) {
        pid_t pid = fork();
        if (pid < 0){
            exit(1);
        } else if (pid > 0) {
            amount += wait_sons();
        } else if (!pid) {
            work(argv[i]);
        }
    }
    
    printf("%d\n", amount);
    return 0;
}