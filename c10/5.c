#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <limits.h>

enum
{
    MAX_LEN = sizeof(time_t) * CHAR_BIT + sizeof(long) * CHAR_BIT,
    PERMISSIONS = 0700,
};

const char path_variable_first[] = "XDG_RUNTIME_DIR";
const char path_variable_second[] = "TMPDIR";
const char begin[] = "#include <stdio.h>\n"
        "int main(void) {\n"
        "char reject[] = \"reject\";\n"
        "char summon[] = \"summon\";\n"
        "char disqualify[] = \"disqualify\";\n"
        "int x;\n" 
        "while (scanf(\"%d\",&x)>0) {\n"
        "printf(\"%s\\n\",";
const char end[] = ");\n"
        "}\n"
        "return 0;\n"
        "}\n";

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
    read(fd_rand, &rand_name, sizeof(rand_name));
    close(fd_rand);
    if (snprintf(name, PATH_MAX, "%s/%llu.c", path, rand_name) >= PATH_MAX) {
        exit(1);
    }
    
    FILE *file = fopen(name, "w");
    if (!file) {
        exit(1);
    }
    if (chmod(name, PERMISSIONS) < 0) {
        exit(1);
    }

    if (fprintf(file, "%s", begin) < sizeof(begin - 1)) {
        fprintf(stderr, "Writing mistake\n");
        exit(1);
    }
    for (int i = 1; i < argc; ++i) {
        if (fprintf(file, "%s", argv[i]) < strlen(argv[i])) {
            fprintf(stderr, "Writing mistake\n");
            exit(1);
        }
    }
    if (fprintf(file, "%s", end) < sizeof(end) - 1) {
        fprintf(stderr, "Writing mistake\n");
        exit(1);
    }
    if (fclose(file) == EOF) {
        exit(1);
    }
    char cmd[PATH_MAX] = "";
    snprintf(cmd, PATH_MAX, "%s/%llu", path, rand_name);

    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        execlp("gcc", "gcc", "-o", cmd, name, NULL);
        _exit(1);
    } else {
        wait(NULL);
        pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            execl("/bin/sh", "sh", "-c", cmd, NULL);
            _exit(1);
        } else {
            wait(NULL);
            if (unlink(name) < 0) {
                exit(1);
            }
            if (unlink(cmd) < 0) {
                exit(1);
            }
        }
    }

    return 0;
}