#include <stdio.h>
#include <stdlib.h>

enum
{
    RADIX = 10,
};

int 
main(int argc, char *argv[]) {
    long long positive_sum = 0;
    long long negative_sum = 0;
    char *end;
    for (int i = 1; i < argc; ++i) {
        int curr = strtol(argv[i], &end, RADIX);
        if (curr > 0) {
            positive_sum += curr;
        } else if (curr < 0) {
            negative_sum += curr;
        }    
    }
    printf("%lld\n%lld\n", positive_sum, negative_sum);
    return 0;
}