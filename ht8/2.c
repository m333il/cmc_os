#include <stdlib.h>
 
enum 
{   
    A = 1103515245, 
    C = 12345, 
    M = 1 << 31,
};

typedef struct RandomGenerator 
{
    const struct RandomOperations *ops;
    unsigned curr;
    unsigned multiplicator;
    unsigned increment;
    unsigned mod;
} RandomGenerator;
 
typedef struct RandomOperations 
{
    unsigned (*next)(RandomGenerator *rr);
    void (*destroy)(RandomGenerator *rr);
} RandomOperations;
 
static void destroy(RandomGenerator *rr) {
    free(rr);
}
 
static unsigned next(RandomGenerator *rr) {
    return rr->curr = (rr->multiplicator * rr->curr + rr->increment) % rr->mod;
}

static const RandomOperations operations = {next, destroy};

RandomGenerator *random_create(unsigned seed) {
    RandomGenerator *new_random = calloc(1, sizeof(*new_random));
    new_random->curr = seed;
    new_random->multiplicator = A;
    new_random->increment = C;
    new_random->mod = M;
    new_random->ops = &operations;
    return new_random;
}