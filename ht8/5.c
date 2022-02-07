#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>

enum
{
    ARGMIN = 2,
    PATH_IND = 1,
};

int cmp(const void *a1, const void *a2) {
    return strcasecmp(*(const char **)a1, *(const char **)a2);
}

void free_array(char *arr[], int n) {
    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);
}

void traversal(char *path) {
    char name[PATH_MAX] = "";
    DIR *dir;
    struct dirent *curr_dir;
    struct stat s;

    int amount = 0;
    int len = 1;
    char **dirs = calloc(len, sizeof(*dirs));

    dir = opendir(path);
    if (!dir) {
        _exit(1);
    }
    while ((curr_dir = readdir(dir))) {
        if (!strcmp(curr_dir->d_name, ".") || !strcmp(curr_dir->d_name, "..")) {
            continue;
        }
        if (snprintf(name, PATH_MAX, "%s/%s", path, curr_dir->d_name) < PATH_MAX 
                && lstat(name, &s) == 0 && S_ISDIR(s.st_mode) && !access(name, R_OK)) {
            int access_status;
            if ((access_status = access(name, R_OK)) < 0) {
                free_array(dirs, amount);
                closedir(dir);
                _exit(1);
            } else if (!access_status) {
                ++amount;
                if (amount > len) {
                    len *= 2;
                    dirs = realloc(dirs, len * sizeof(*dirs));
                    if (!dirs) {
                        free_array(dirs, amount);
                        closedir(dir);
                        _exit(1);
                    }
                }
                dirs[amount - 1] = strdup(curr_dir->d_name);
                if (!dirs[amount - 1]) {
                    free_array(dirs, amount);
                    closedir(dir);
                    _exit(1);
                }
            }
        }
    }
    closedir(dir);
    if (amount) {
        qsort(dirs, amount, sizeof(*dirs), cmp);
    }
    for (int i = 0; i < amount; ++i) {
        printf("cd %.*s\n", strlen(dirs[i]), dirs[i]);
        if (snprintf(name, PATH_MAX, "%s/%s", path, dirs[i]) > PATH_MAX) {
            continue;
        }
        traversal(name);
        printf("cd ..\n");
    }

    free_array(dirs, amount);
}

int main(int argc, char *argv[]) {
    if (argc < ARGMIN) {
        return 0;
    }
    traversal(argv[PATH_IND]);
    return 0;
}