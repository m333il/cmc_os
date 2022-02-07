#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

enum
{
    FIRST_BYTE = 4,
    SECOND_BYTE = 12,
    THIRD_BYTE = 20,
    FOURTH_BYTE = 24,
    MOVE_RIGNT = 24,
    MOVE_TO_MAKE_ZERO = 4,
    ARG_MIN = 2,
    AMOUNT_OF_BYTES = 4,
};

void
change_bits(unsigned n, uint8_t *num) {
    num[FIRST_BYTE / CHAR_BIT] = (n << FIRST_BYTE) >> MOVE_RIGNT;
    num[SECOND_BYTE / CHAR_BIT] = (n << SECOND_BYTE) >> MOVE_RIGNT;
    num[THIRD_BYTE / CHAR_BIT] = ((n << THIRD_BYTE) >> MOVE_TO_MAKE_ZERO) >> MOVE_RIGNT;
    num[FOURTH_BYTE / CHAR_BIT] = (n << FOURTH_BYTE) >> MOVE_RIGNT; 
}

int 
main(int argc, char *argv[]) {
    int file;
    if (argc < ARG_MIN || (file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
        exit(1);
    }
    unsigned curr;

    while (scanf("%u", &curr) == 1) {
        uint8_t num[AMOUNT_OF_BYTES] = {0};
        change_bits(curr, num);
        if (write(file, num, sizeof(curr)) != sizeof(curr)) {
            exit(1);
        }
    }
    close(file);
    return 0;
}