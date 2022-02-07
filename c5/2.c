#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

const char str[] = "rwxrwxrwx";

enum
{
    STR_SIZE = sizeof(str) - 1,
    BASE = 8,
};

int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        int curr = 0;
        char *endptr;
        curr = strtol(argv[i], &endptr, BASE);
        for (int j = STR_SIZE - 1; j >= 0; --j) {
            if ((curr >> j) & 1) {
                putc(str[STR_SIZE - j - 1], stdout);
            } else {
                putc('-', stdout);
            }
        }
        putc('\n', stdout);
    }
    return 0;
}