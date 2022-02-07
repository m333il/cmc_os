#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

enum
{
    MOD = 2 << 10,
};

int 
main(int argc, char *argv[]) {
    unsigned long long sum = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat st;
        if (lstat(argv[i], &st) != -1 && st.st_nlink == 1 && S_ISREG(st.st_mode)) {
            off_t size = st.st_size;
            if (!(size % MOD)) {
                sum += size;
            }
        }
    }
    printf("%llu\n", sum);
    return 0;
}
