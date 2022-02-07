#include <stdio.h>
#include <time.h>

enum
{
    SEC_IN_DAY = 60 * 60 * 24,
    START_YEAR = 1900,
    MOD = 3,
    TUESDAY1 = 2,
    TUESDAY2 = 4,
};

int main() {
    int year;
    scanf("%d", &year);
    struct tm s;
    s.tm_year = year - START_YEAR;
    s.tm_mon = 0;
    s.tm_mday = 1;
    s.tm_hour = 0;
    s.tm_min = 0;
    s.tm_sec = 0;
    s.tm_isdst = -1;
    
    int curr_tuesday = 0;
    time_t curr = timegm(&s);
    while (s.tm_year == year - START_YEAR) {
        gmtime_r(&curr, &s);
        if (s.tm_mday == 1) {
            curr_tuesday = 0;
        }
        if (s.tm_wday == 4) {
            ++curr_tuesday;
            if (curr_tuesday == TUESDAY1 || curr_tuesday == TUESDAY2) {
                if (s.tm_mday % MOD) {
                    printf("%d %d\n", s.tm_mon + 1, s.tm_mday);
                }
            }
        }
        curr += SEC_IN_DAY;
    }

    return 0;
}