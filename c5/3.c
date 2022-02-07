#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>

enum
{
    MAX_BITS = 32,
};

int main(void) {
    uint32_t n, s, w;
    scanf("%" SCNu32 "%" SCNu32 "%" SCNu32, &n, &s, &w);
    uint32_t end = (n == MAX_BITS ? UINT32_MAX : 1u << n);
    
    for (uint32_t i = 0; i < end; i += s) {
        uint32_t r = (n == 1 ? 0 : ((i << (MAX_BITS - n + 1)) >> (MAX_BITS - n + 1)));
        if ((i >> (n - 1)) & 1) {
            r = -r;
        }
        printf("|%*" PRIo32 "|%*" PRIu32 "|%*" PRId32 "|\n", (int) w, i, (int) w, i, (int) w, r);
        if (UINT32_MAX - i < s) {
            break;
        }
    }
    return 0;
}