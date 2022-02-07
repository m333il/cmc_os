#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

enum
{
    USEC_IN_SEC = 1000000,
    TV_SEC_INDEX = 1,
    TV_USEC_INDEX = 2,
    H_INDEX = 3,
    K_INDEX = 4,
    S_INDEX = 5,
};

int main(int argc, char *argv[]) {
    struct timeval t0;
    char *endptr = NULL;
    t0.tv_sec = strtol(argv[TV_SEC_INDEX], &endptr, 0);
    t0.tv_usec = strtol(argv[TV_USEC_INDEX], &endptr, 0);
    double h = strtod(argv[H_INDEX], &endptr);
    unsigned int k = strtoul(argv[K_INDEX], &endptr, 0);
    unsigned int s = strtoul(argv[S_INDEX], &endptr, 0);

    srand(s);
    for (unsigned int i = 0; i < k; ++i) {
        double u = (double) rand() / RAND_MAX;
        double delta = floor(- log(u) / h);
        t0.tv_usec += delta;
        t0.tv_sec += t0.tv_usec / USEC_IN_SEC;
        t0.tv_usec %= USEC_IN_SEC;
        printf("%ld %ld\n", t0.tv_sec, t0.tv_usec);
    }

    return 0;
}
