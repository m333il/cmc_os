#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

enum
{
    BITS = sizeof(int64_t) * CHAR_BIT,
};

int adding_overflow(int64_t a, int64_t b, int64_t *res) {
    if (a > INT64_MAX - b) {
        *res = 1;
        return 1;
    } else {
        *res = a + b;
        return 0;
    }
}

int multiplication_overflow(int64_t a, int64_t b, int64_t *res) {
    if (a > INT64_MAX / b) {
        *res = 1;
        return 1;
    } else {
        *res = a * b;
        return 0;
    }
}

int64_t calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count) {
    int overflow_a = 0;
    int overflow_b = 0;
    int64_t a = 1;
    int64_t b;

    int64_t t = 1;
    overflow_a += multiplication_overflow(block_num_size, 8, &t);
    if (t >= BITS) {
        overflow_a += 1;
    } else {
        a <<= t;
        overflow_a += multiplication_overflow(a, block_size, &a);
    }

    int64_t first_level = block_size / block_num_size;
    int64_t second_level, third_level;
    overflow_b += multiplication_overflow(first_level, first_level, &second_level);
    overflow_b += multiplication_overflow(second_level, first_level, &third_level);
    overflow_b += adding_overflow(inode_direct_block_count, first_level, &b);
    overflow_b += adding_overflow(b, second_level, &b);
    overflow_b += adding_overflow(b, third_level, &b);
    overflow_b += multiplication_overflow(b, block_size, &b);

    if (overflow_b && overflow_a) {
        return -1;
    } else if (overflow_a) {
        return b;
    } else if (overflow_b) {
        return a;
    }
    return a < b ? a : b;
}

int main(int argc, char *argv[]) {
    int64_t block_size, block_num_size, inode_direct_block_count;
    scanf("%" SCNd64 "%" SCNd64 "%" SCNd64, &block_size, &block_num_size, &inode_direct_block_count);
    printf("%" PRId64 "\n", calc_max_size(block_size, block_num_size, inode_direct_block_count));
    int64_t block_max = 0, size_max = INT64_MIN;
    for (uint64_t i = 1; i <= block_size; ++i) {
        int64_t curr_size = calc_max_size(block_size, i, inode_direct_block_count);
        if (curr_size == -1) {
            size_max = -1;
            block_max = i;
            break;
        }
        if (curr_size > size_max) {
            size_max = curr_size;
            block_max = i;
        } else if (curr_size < size_max) {
            break;
        }
    }
    printf("%" PRId64 " %" PRId64 "\n", block_max, size_max);
    return 0;
}