/*****************************************************************
 * @file utc_convert.c
 * @author boaz (hubocun@yolanda.hk)
 * @brief 实现
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 ****************************************************************/

#include "utc_convert.h"

static const uint8_t days_per_mon[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

#define SEC_PER_DAY          86400UL
#define IsLeapYear(yr)       (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define DAYS_PER_YEAR(yr)    (IsLeapYear(yr) ? 366 : 365)
#define DAYS_PER_MON(yr, mh) (IsLeapYear(yr) ? days_per_mon[1][mh] : days_per_mon[0][mh])

/*****************************************************************
 * @brief UTC转换为年月日时分秒
 * 
 * @param sec_time UTC时间
 * @return 返回时间结构体指针
 ****************************************************************/
utc_time_t * utc_to_time(utc_sec *sec_time)
{
    utc_time_t *tm;
    uint16_t year_per_days = 0;
    uint32_t day = *sec_time % SEC_PER_DAY;
    uint32_t days = *sec_time / SEC_PER_DAY;

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

    return tm;
}

/*****************************************************************
 * @brief 年月日转换为UTC时间
 * 
 * @param tm 时间结构体指针
 * @return 返回UTC时间
 ****************************************************************/
utc_sec time_to_utc(utc_time_t *tm)
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
