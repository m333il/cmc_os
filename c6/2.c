#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

const char forbidden[] = ".exe";

enum
{
    SUFFIX_LEN = sizeof(forbidden) - 1,
};

int main(int argc, char *argv[]) {
    DIR *dp = opendir(argv[1]);
    long long ans = 0;
    struct dirent *curr;
    while ((curr = readdir(dp)) != NULL) {
        struct stat st;
        char name[PATH_MAX];
        unsigned n;
        char sep[2] = "";
        if (strcmp(argv[1], "/")) {
            snprintf(sep, sizeof(sep), "/");
        }
        n = snprintf(name, sizeof(name), "%s%s%s", argv[1], sep, curr->d_name);
        if (n < sizeof(name)) {
            if (!stat(name, &st)) {
                int len = strlen(curr->d_name);
                if (S_ISREG(st.st_mode) && !access(name, X_OK)) {
                    if (len >= SUFFIX_LEN && !strcmp(curr->d_name + len - SUFFIX_LEN, forbidden)) {
                        ++ans;
                    }
                }       
            }
        }
    }
    printf("%lld\n", ans);
    closedir(dp);
    return 0;
}