#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int curr;
    int parent_id = getpid();
    while (scanf("%d", &curr) > 0) {
        int pid = fork();
        if (pid < 0) {
            printf("-1\n");
            fflush(stdout);
            _exit(!(parent_id == getpid()));
        } else if (pid > 0) {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status)) {
                _exit(!(parent_id == getpid()));
            } else {
                printf("%d\n", curr);
                fflush(stdout);
                break;
            }
        }
    }
    return 0;
}