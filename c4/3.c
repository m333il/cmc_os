#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

enum
{
    BUFF_SIZE = 20,
    SICKLE_TO_KNUTS = 29,
    GALLEON_TO_SICKLES = 17,
};

int 
get_num(uint8_t *num) {
    int ans = 0;
    for (int i = 0; i < sizeof(int); ++i) {
        ans |= (unsigned) num[i] << ((sizeof(int) - i - 1) * CHAR_BIT);
    }
    return ans;
}

int main(int argc, char *argv[]) {
    int fl1 = 0, fl2 = 0;
    int max1 = 0, max2 = 0;
    for (int i = 1; i < argc; ++i) {
        int file = open(argv[i], O_RDONLY);
        if (file < 0) {
            close(file);
            continue;
        }
        int curr;
        uint8_t num[BUFF_SIZE];
        while (read(file, num, BUFF_SIZE) == BUFF_SIZE) {
            curr = get_num(num + BUFF_SIZE - sizeof(curr));
            if (!fl1) {
                max1 = curr;
                fl1 = 1;
            } else if (!fl2) {
                if (curr > max1) {
                    max2 = max1;
                    max1 = curr;
                    fl2 = 1;
                } else if (curr < max1) {
                    max2 = curr;
                    fl2 = 1;
                }
            } else if (curr > max1) {
                max2 = max1;
                max1 = curr;
            } else if (curr > max2 && curr != max1) {
                max2 = curr;
            }
        }
        close(file);
    }
    if (fl1 && fl2) {
        int neg = (max2 < 0);
        int g = 0, s = 0, k = 0;
        k = max2 % SICKLE_TO_KNUTS;
        max2 /= SICKLE_TO_KNUTS;
        s = max2 % GALLEON_TO_SICKLES;
        max2 /= GALLEON_TO_SICKLES;
        g = max2;
        if (neg) {
            s = -s;
            k = -k;
        }
        if (!g && neg) {
            putc('-', stdout);
        }
        printf("%dg%02ds%02dk", g, s, k);
    }
    putc('\n', stdout);

    return 0;
}