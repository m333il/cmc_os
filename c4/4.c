#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

enum
{
    STRUCT_SIZE = 3,
    KEY_INDEX = 0,
    LEFT_IDX_INDEX = 1,
    RIGHT_IDX_INDEX = 2,
    ARG_MIN = 2,
    BEGINNING = 1,
    NOT_BEGINNING = 0,
};

void 
search(int file, int pos, int start) {
    if (!pos && !start) {
        return;
    }
    int32_t curr_struct[STRUCT_SIZE];
    if (lseek(file, pos * sizeof(curr_struct), SEEK_SET) == (off_t) -1) {
        exit(1);
    }
    if (read(file, curr_struct, sizeof(curr_struct)) != sizeof(curr_struct)) {
        return;
    }
    if (curr_struct[RIGHT_IDX_INDEX]) {
        search(file, curr_struct[RIGHT_IDX_INDEX], NOT_BEGINNING);
    }
    printf("%d\n", curr_struct[KEY_INDEX]);
    if (curr_struct[LEFT_IDX_INDEX]) {
        search(file, curr_struct[LEFT_IDX_INDEX], NOT_BEGINNING);
    }
}

int 
main(int argc, char *argv[]) {
    int file;
    if (argc < ARG_MIN || (file = open(argv[1], O_RDONLY)) < 0) {
        exit(1);
    }
    search(file, 0, BEGINNING);
    return 0;
}