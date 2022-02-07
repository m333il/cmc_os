#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

enum
{
    AMOUNT_OF_DIGITS = 10,
    BUFF_SIZE = 4096 + 1,
};

void 
print(unsigned long long *arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d %llu\n", i, arr[i]);
    }
}

int 
main(void) {
    unsigned long long digits[AMOUNT_OF_DIGITS] = {0};
    char filename[BUFF_SIZE];
    if (fgets(filename, sizeof(filename), stdin) == NULL && ferror(stdin)) {
        exit(0);
    }
    filename[strcspn(filename, "\n\r")] = '\0';
    if (strlen(filename) == 0) {
        print(digits, AMOUNT_OF_DIGITS);
        return 0;
    }
    int file = open(filename, O_RDONLY);
    if (file < 0) {
        print(digits, AMOUNT_OF_DIGITS);
        return 0;
    }
    char buff[BUFF_SIZE];
    while (read(file, buff, sizeof(buff)) > 0) {
        int len = strlen(buff);
        for (int i = 0; i < len; ++i) {
            if (buff[i] >= '0' && buff[i] <= '9') {
                ++digits[buff[i] - '0'];
            }
        }
        memset(buff, 0, BUFF_SIZE);
    }
    close(file);
    print(digits, AMOUNT_OF_DIGITS);
    return 0;
}