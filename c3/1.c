#include <stdio.h>
#include <limits.h>

typedef int STYPE;
typedef unsigned int UTYPE;

enum {AMOUNT_OF_BITS = sizeof(STYPE) * CHAR_BIT};

STYPE bit_reverse(STYPE value) {
    UTYPE ans = 0, n = (UTYPE) value;
    for (size_t i = 0; i < AMOUNT_OF_BITS; ++i) {
        ans = (ans << 1) | (n & 1);
        n >>= 1;
    }
    return (STYPE) ans;
}
