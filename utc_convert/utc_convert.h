/*****************************************************************
 * @file utc_convert.h
 * @author boaz (hubocun@yolanda.hk)
 * @brief 定义
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 ****************************************************************/

#ifndef __UTC_CONVERT_H
#define __UTC_CONVERT_H

#include "stdint.h"

#define BASE_YEAR   1970   /* 基于1970-1-1 0:0:0 */

typedef uint32_t utc_sec;
typedef struct utc_time_t
{
    uint16_t year;  /* 实际年份，不需要额外 + 1970 */
    uint8_t mon;    /* 实际月份，不需要额外 + 1 */
    uint8_t day;
    uint8_t min;
    uint8_t hour;
    uint8_t sec;
} utc_time_t;

void utc_to_time(utc_sec utc, utc_time_t *tm);
utc_sec time_to_utc(utc_time_t *tm);

#endif // __UTC_CONVERT_H
