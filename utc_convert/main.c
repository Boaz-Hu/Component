/*****************************************************************
 * @file main.c
 * @author boaz (hubocun@yolanda.hk)
 * @brief 函数测试
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 ****************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "utc_convert.h"

int main(void)
{
    utc_time_t *tm;
    uint32_t sec_time = 946684800;  /* 2000-1-1 0:0:0 */

    // tm = utc_to_time(&sec_time);
    // printf("%d-%d-%d %d:%d:%d\n", tm->year, tm->mon, tm->day, tm->hour, tm->min, tm->sec);

    // sec_time = time_to_utc(tm);
    // printf("rtc value : %d\n", sec_time);

    struct tm *c_tm;
    time_t c_sec_time = 946684800;

    // c_tm = gmtime(&c_sec_time);  /* 不带时区计算 */
    // c_tm = localtime(&c_sec_time);  /* 带时区计算 */
    // printf("gmtime:    %d-%d-%d %d:%d:%d\n", c_tm->tm_year+1900, c_tm->tm_mon+1, c_tm->tm_mday, c_tm->tm_hour, c_tm->tm_min, c_tm->tm_sec);

    // c_sec_time = mktime(c_tm);  /* 带时区计算 */
    // printf("mktime:    %d\n", c_sec_time);

    printf("testing");
    for (long i = 0, dot = 0; i < 31622400; i++) /* 100 years */
    {
        if (!(dot++ % 316224)) {
            printf(".");
        }

        c_tm = gmtime(&c_sec_time);
        tm = utc_to_time(&sec_time);
        if ((c_tm->tm_year+1900) != tm->year
            || (c_tm->tm_mon+1) != tm->mon
            || c_tm->tm_mday != tm->day
            || c_tm->tm_hour != tm->hour
            || c_tm->tm_min != tm->min
            || c_tm->tm_sec != tm->sec) {
            printf("Error: utc_to_time\n");
            printf("gmtime:      %d-%d-%d %d:%d:%d\n", c_tm->tm_year+1900, c_tm->tm_mon+1, c_tm->tm_mday, c_tm->tm_hour, c_tm->tm_min, c_tm->tm_sec);
            printf("utc_to_time: %d-%d-%d %d:%d:%d\n", tm->year, tm->mon, tm->day, tm->hour, tm->min, tm->sec);
            printf("mktime:      %d\n", c_sec_time);
            printf("time_to_utc: %d\n", sec_time);
            break;
        }
        c_sec_time = mktime(c_tm) + 28800UL;    /* 加上东八区 */
        sec_time = time_to_utc(tm);
        if (c_sec_time != sec_time) {
            printf("Error: time_to_utc\n");
            printf("gmtime:      %d-%d-%d %d:%d:%d\n", c_tm->tm_year+1900, c_tm->tm_mon+1, c_tm->tm_mday, c_tm->tm_hour, c_tm->tm_min, c_tm->tm_sec);
            printf("utc_to_time: %d-%d-%d %d:%d:%d\n", tm->year, tm->mon, tm->day, tm->hour, tm->min, tm->sec);
            printf("mktime:      %d\n", c_sec_time);
            printf("time_to_utc: %d\n", sec_time);
            break;
        }
        c_sec_time++;
        sec_time++;
    }
    printf("\nend\n");
    scanf("");

    return 0;
}