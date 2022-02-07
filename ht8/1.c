#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

enum
{
    BASE = 16,
    FILE_IND = 1,
    START_IND = 2,
    PAGE_SIZE = 512,
    FILE_SIZE = 65536,
};

int main(int argc, char *argv[]) {
    int fd;
    if ((fd = open(argv[FILE_IND], O_RDONLY)) < 0) {
        exit(1);
    } 
    struct stat st;
    if (fstat(fd, &st) < 0) {
        exit(1);
    }
    if (st.st_size != FILE_SIZE) {
        exit(1);
    }
    unsigned short *memory = mmap(NULL, FILE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        exit(1);
    }
    if (close(fd) < 0) {
        exit(1);
    }
    int offset = strtol(argv[START_IND], NULL, BASE) / sizeof(short);
    unsigned short *start = memory + offset;
    unsigned short curr;
    while (scanf("%hxu", &curr) > 0) {
        int ind = start[curr / PAGE_SIZE] + curr % PAGE_SIZE;
        ind /= sizeof(short);
        printf("%u\n", memory[ind]);
    }
    if (munmap(memory, sizeof(memory)) < 0) {
        exit(1);
    }
    return 0;
}
