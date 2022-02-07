#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

enum
{
    MOD = 2,
    ARG_MIN = 2,
};

int 
main(int argc, char *argv[]) {
    int fd;
    if (argc < ARG_MIN || (fd = open(argv[1], O_RDONLY)) < 0) {
        exit(1);
    }
    uint16_t ans = 0, num = 0;
    uint8_t curr[sizeof(ans)] = {0};
    int fl = 0;
    while (read(fd, curr, sizeof(ans)) == sizeof(ans)) {
        num = curr[1] | (curr[0] << CHAR_BIT);
        if (num % MOD == 0) {
            if (!fl) {
                ans = num;
                fl = 1;
            } else if (num == 0) {
                ans = 0;
                break;
            } else if (num < ans) {
                ans = num;
            }
        }
    }
    close(fd);
    if (fl) {
        printf("%" PRIu16 "\n", ans);
    }
    return 0;
}