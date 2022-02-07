#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    PERCENTAGE = 100,
    START = 2,
    ROUND = 10000,
};

int 
main(int argc, char *argv[]) {
    char *ptr;
    double value = strtod(argv[1], &ptr);
    for (int i = START; i < argc; ++i) {
        value *= (PERCENTAGE * 1.0 + strtod(argv[i], &ptr)) / (PERCENTAGE * 1.0);
        value = round(ROUND * value) / ROUND; 
    }
    printf("%.4lf\n", value);
    return 0;
}