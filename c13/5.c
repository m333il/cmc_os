#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

volatile char flag = 0;

void handler(int s) {
    flag = s;
}

int main(int argc, char *argv[]) {

    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);

    sigset_t sigs, old_sigs;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGUSR1);
    sigaddset(&sigs, SIGUSR2);
    sigaddset(&sigs, SIGIO);
    sigprocmask(SIG_BLOCK, &sigs, &old_sigs);
    
    pid_t pid1 = fork();
    if (pid1 < 0) {
        exit(1);
    } else if (!pid1) {
        unsigned char curr_byte = 0;
        int cnt = 0;
        while (1) {
            while (!flag) {
                sigsuspend(&old_sigs);
            }
            if (flag == SIGUSR1) {
                if (++cnt != CHAR_BIT) {
                    curr_byte <<= 1;
                } 
            } else if (flag == SIGUSR2) {
                curr_byte |= 1u;
                if (++cnt != CHAR_BIT) {
                    curr_byte <<= 1;
                }
            } else if (flag == SIGIO) {
                _exit(0);
            } else if (flag == SIGALRM) {
                flag = 0;
                continue;
            }
            flag = 0;
            if (cnt == CHAR_BIT) {
                putc(curr_byte, stdout);
                fflush(stdout);
                curr_byte = 0;
                cnt = 0;
            }
            kill(0, SIGALRM);
        }
    }
    
    pid_t pid2 = fork();
    if (pid2 < 0) {
        exit(1);
    } else if (!pid2) {
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            kill(pid1, SIGIO);
            _exit(1);
        }
        unsigned char curr_byte;
        while (read(fd, &curr_byte, sizeof(curr_byte)) > 0) {
            for (int i = 0; i < CHAR_BIT; ++i) {
                unsigned char mask = 1u << (CHAR_BIT - 1 - i);
                int curr_bit = curr_byte & mask;
                if (curr_bit) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }
                while (!flag) {
                    sigsuspend(&old_sigs);
                }
                flag = 0;
            }
        }
        kill(pid1, SIGIO);
        if (close(fd) < 0) {
            _exit(1);
        }
        exit(0);
    }
    
    while (wait(NULL) > 0)
        ;
    
    return 0;
}