#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

enum
{
    MEMORY_IND = 1,
    CACHE_IND = 2,
    BLOCK_IND = 3,
    BASE = 10,
};


int main(int argc, char *argv[]) {
    int miss = 0;
    int cache_size = strtol(argv[CACHE_IND], NULL, BASE);
    int block_size = strtol(argv[BLOCK_IND], NULL, BASE);

    int blocks_in_cache = cache_size / block_size;

    int *blocks = calloc(blocks_in_cache, sizeof(*blocks));
    for (int i = 0; i < blocks_in_cache; ++i) {
        blocks[i] = -1;
    }

    char cmd[3] = "";
    while (scanf("%3s", cmd) > 0) {
        unsigned addr, size;
        int value;
        scanf("%x %u %d", &addr, &size, &value);
        unsigned block = addr / block_size;

        unsigned ind = block % blocks_in_cache;
        if (cmd[0] == 'W') {
            blocks[ind] = block;
        } else if (cmd[0] == 'R') {
            if (block != blocks[ind]) {
                if (blocks[ind] >= 0) {
                    ++miss;
                }
            }
            blocks[ind] = block;
        }
    }
    printf("%d\n", miss);
    free(blocks);
    return 0;
}
