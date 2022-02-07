#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

enum
{
    BUFF_SIZE = 251,
    AMOUNT_OF_PARTIES = 6,
};

void change_back(short prev_sum, short prev_sum_right, short count_prev, short *prev) {
    short count_other = 0;
    for (int i = 0; i < count_prev; ++i) {
        if (prev[i] > 1) {
            ++prev[i];
            ++count_other;
        }
    }
    short change_ones = prev_sum_right - prev_sum - count_other;
    for (int i = 0, j = 0; i < count_prev && j < change_ones; ++i) {
        if (prev[i] == 1) {
            ++prev[i];
            ++j;
        }
    }
}

int main(int argc, char **argv) {
    int fd;
    if (argc < 2 || (fd = open(argv[1], O_RDONLY)) < 0) {
        return 0;
    }

    int party[AMOUNT_OF_PARTIES] = {0};
    short prev[BUFF_SIZE] = {0};
    short prev_sum = 0, prev_sum_right = 0;
    short count = 0, count_prev = 0;;
    read(fd, &count, sizeof(short));
    while (count) {
        read(fd, &prev_sum_right, sizeof(short));
        prev_sum = 0;
        for (int i = 0; i < count_prev; ++i) {
            prev_sum += prev[i];
        }
        if (prev_sum_right != prev_sum) {
            change_back(prev_sum, prev_sum_right, count_prev, prev);
        }
        for (int i = 0; i < count_prev; ++i) {
            ++party[prev[i] - 1];
        }
        for (int i = 0; i < count; ++i) {
            read(fd, &prev[i], sizeof(short));
        }
        count_prev = count;
        read(fd, &count, sizeof(short));
    }
    read(fd, &prev_sum_right, sizeof(short));
    prev_sum = 0;
    for (int i = 0; i < count_prev; ++i) {
        prev_sum += prev[i];
    }
    if (prev_sum_right != prev_sum) {
        change_back(prev_sum, prev_sum_right, count_prev, prev);
    }
    for (int i = 0; i < count_prev; ++i) {
        ++party[prev[i] - 1];
    }
    for (int i = 0; i < AMOUNT_OF_PARTIES; ++i)
        printf("%d\n", party[i]);
    return 0;
}
