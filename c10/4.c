#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

enum
{
    MAX_LEN = sizeof(time_t) * CHAR_BIT + sizeof(long) * CHAR_BIT,
    PERMISSIONS = 0700,
};

const char begin[] = "#! /usr/bin/python3\n"
        "import os\n"
        "print(";
const char end[] = ")\n"
        "os.remove(__file__)";
const char path_variable_first[] = "XDG_RUNTIME_DIR";
const char path_variable_second[] = "TMPDIR";

extern char **environ;

int main(int argc, char *argv[]) {
    char *path = getenv(path_variable_first);
    if (!path) {
        path = getenv(path_variable_second);
        if (!path) {
            path = "/tmp";
        }
    }
    char name[PATH_MAX] = "";
    unsigned long long rand_name = 0;
    int fd_rand = open("/dev/urandom", O_RDONLY);
    if (fd_rand < 0) {
        exit(1);
    }
    if (read(fd_rand, &rand_name, sizeof(rand_name)) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    close(fd_rand);
    if (snprintf(name, PATH_MAX, "%s/%llu.py", path, rand_name) >= PATH_MAX) {
        exit(1);
    }
    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS);
    if (fd < 0) {
        exit(1);
    }
    
    if (write(fd, begin, sizeof(begin) - 1) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    for (int i = 1; i < argc; ++i) {
        if (write(fd, argv[i], strlen(argv[i])) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }
        if (i != argc - 1) {
            char mul = '*';
            if (write(fd, &mul, sizeof(mul)) < 0) {
                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);
            }
        }
    }
    if (write(fd, end, sizeof(end) - 1) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    if (close(fd) < 0) {
        exit(1);
    }
    execlp(name, name, NULL);
    return 1;
}