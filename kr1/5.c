#include <stdio.h>
#include <time.h>

enum
{
    SEC_BETWEEN_FULL_MOON = 29 * 24 * 60 * 60 + 12 * 60 * 60 + 44 * 60,
    START_YEAR = 1900,
    FULL_MOON_YEAR = 2021,
    FULL_MOON_MONTH = 5,
    FULL_MOON_DAY = 26,
    FULL_MOON_HOUR = 11,
    FULL_MOON_MIN = 44,
    FULL_MOON_SEC = 0,
    SEC_IN_DAY = 24 * 60 * 60,
    AMOUNT_MONDAYS = 3,
    MON = 0,
};

int
main() {
    int year;
    scanf("%d", &year);
    struct tm full_moon;
    full_moon.tm_year = FULL_MOON_YEAR - START_YEAR;
    full_moon.tm_mon = FULL_MOON_MONTH;
    full_moon.tm_mday = FULL_MOON_DAY;
    full_moon.tm_hour = FULL_MOON_HOUR;
    full_moon.tm_min = FULL_MOON_MIN;
    full_moon.tm_sec = FULL_MOON_SEC;
    full_moon.tm_isdst = -1;
    time_t full_moon_t = timegm(&full_moon);
    struct tm s;
    s.tm_year = year - START_YEAR;
    s.tm_mon = 0;
    s.tm_mday = 1;
    s.tm_hour = 0;
    s.tm_min = 0;
    s.tm_sec = 0;
    s.tm_isdst = -1;
    time_t curr = timegm(&s);

    int step = curr > full_moon_t ? SEC_BETWEEN_FULL_MOON : -SEC_BETWEEN_FULL_MOON;

    while (1) {
        if (step > 0) {
            if (full_moon.tm_year + START_YEAR == year) {
                break;
            }
            full_moon_t += SEC_BETWEEN_FULL_MOON;
        } else {
            if (full_moon.tm_year + START_YEAR < year) {
                full_moon_t += SEC_BETWEEN_FULL_MOON;
                gmtime_r(&full_moon_t, &full_moon);
                break;
            }
            full_moon_t -= SEC_BETWEEN_FULL_MOON;
        }
        gmtime_r(&full_moon_t, &full_moon);
    }
    while (full_moon.tm_yday <= 256) {
        full_moon_t += SEC_BETWEEN_FULL_MOON;
        gmtime_r(&full_moon_t, &full_moon);
    }
    int count_mon = AMOUNT_MONDAYS + 1;
    while (count_mon) {
        if (full_moon.tm_wday == MON) {
            --count_mon;   
        }
        full_moon_t += SEC_IN_DAY;
        gmtime_r(&full_moon_t, &full_moon);
    }
    printf("%04d-%02d-%02d\n", full_moon.tm_year + START_YEAR, full_moon.tm_mon + 1, full_moon.tm_mday);
    return 0;
}
