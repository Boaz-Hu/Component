#include <stdio.h>
#include <stdint.h>

static uint8_t days_per_mon[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

#define BASE_YEAR            1970
#define SEC_PER_DAY          86400UL
#define IsLeapYear(yr)       (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define DAYS_PER_YEAR(yr)    (IsLeapYear(yr) ? 366 : 365)
#define DAYS_PER_MON(yr, mh) (IsLeapYear(yr) ? days_per_mon[1][mh] : days_per_mon[0][mh])

typedef uint32_t utc_time;

typedef struct utc_time_t
{
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t min;
    uint8_t hour;
    uint8_t sec;
} utc_time_t;

void my_localtime(utc_time sec_time, utc_time_t *tm)
{
    uint16_t year_per_days = 0;
    uint32_t day = sec_time % SEC_PER_DAY;
    uint32_t days = sec_time / SEC_PER_DAY;

    tm->sec = day % 60UL;
    tm->min = (day % 3600UL) / 60UL;
    tm->hour = day / 3600UL;
    /* year */
    tm->year = BASE_YEAR;
    while (days >= DAYS_PER_YEAR(tm->year))
    {
        days -= DAYS_PER_YEAR(tm->year);
        tm->year++;
    }
    /* month */
    tm->mon = 0;
    while (days >= DAYS_PER_MON(tm->year, tm->mon))
    {
        days -= DAYS_PER_MON(tm->year, tm->mon);
        tm->mon++;
    }
    /* day */
    tm->day = days;

    tm->mon++;
    tm->day++;
}

utc_time my_mktime(utc_time_t *tm)
{
    uint32_t seconds = (((tm->hour * 60UL) + tm->min) * 60UL) + tm->sec;
    uint16_t year = tm->year;
    int8_t month = tm->mon;
    uint16_t days = tm->day;

    month--;
    days--;

    while (--month >= 0)
    {
        days += DAYS_PER_MON(tm->year, month);
    }

    while (--year >= BASE_YEAR)
    {
        days += DAYS_PER_YEAR(year);
    }

    seconds += (days * SEC_PER_DAY);

    return seconds;
}

int main(void)
{
    utc_time_t tm;
    uint32_t sec_time = 952649808;

    my_localtime(sec_time, &tm);
    printf("%d-%d-%d %d:%d:%d\n", tm.year, tm.mon, tm.day, tm.hour, tm.min, tm.sec);

    sec_time = my_mktime(&tm);
    printf("rtc value : %d\n", sec_time);

    return 0;
}