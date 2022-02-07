#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void) {
    int pid1 = fork();
    if (!pid1) {
        int pid2 = fork();
        if (!pid2) {
            printf("3 ");
            fflush(stdout);  
            _exit(0);
        } else {
            wait(NULL);
            printf("2 ");
            fflush(stdout);
        }
        _exit(0);
    } else {
        wait(NULL);
        printf("1\n");
    }
    return 0;
}