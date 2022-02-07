#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

enum
{
    ARG_MIN = 2,
};

int 
main(int argc, char *argv[]) {
    int fd;
    if (argc < ARG_MIN || (fd = open(argv[1], O_RDWR)) < 0) {
        exit(1);
    }
    int fl = 0;
    long long min_num;
    long pos = 0;
    long pos_curr = 0;
    long long curr;
    while (read(fd, &curr, sizeof(curr)) == sizeof(curr)) {
        if (!fl) {
            min_num = curr;
            fl = 1;
        }
        if (curr < min_num) {
            min_num = curr;
            pos = pos_curr;
        }
        ++pos_curr;
    }
    if (lseek(fd, pos * sizeof(min_num), SEEK_SET) == (off_t) -1) {
        exit(1);
    }
    if (min_num != LLONG_MIN) {
        min_num = -min_num;
    }
    if (fl) {
        if (write(fd, &min_num, sizeof(min_num)) != sizeof(min_num)) {
            exit(1);
        }
    }
    close(fd);
    return 0;
}