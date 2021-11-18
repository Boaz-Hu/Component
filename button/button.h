#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stdint.h"

#define BUTTON_SCAN_FREQ_HZ    50                                  /* 20ms扫描周期 */
#define BUTTON_MS_TO_TICKS(ms) (ms / (1000 / BUTTON_SCAN_FREQ_HZ)) /* ms转节拍 */

#define MAX_MULTIPLE_CLICKS_INTERVAL  BUTTON_MS_TO_TICKS(300)  /* 连击间隔 */
#define BUTTON_SHORT_PRESS_START_TICK BUTTON_MS_TO_TICKS(1500) /* 短按判断时间 */
#define BUTTON_LONG_PRESS_START_TICK  BUTTON_MS_TO_TICKS(3000) /* 长按判断时间 */
#define BUTTON_LONG_HOLD_START_TICK   BUTTON_MS_TO_TICKS(4500) /* 长按保持判断时间 */

/* 按键组 */
typedef enum user_button_t
{
    BUTTON_WK_UP,
    BUTTON_KEY0,
    BUTTON_KEY1,
    BUTTON_MAX
} user_button_t;

typedef enum button_event_t
{
    BUTTON_EVENT_DOWN = 0,     // 按下事件
    BUTTON_EVENT_CLICK,        // 单击事件，常用
    BUTTON_EVENT_DOUBLE_CLICK, // 双击事件，常用
    BUTTON_EVENT_REPEAT_CLICK, // 连击事件，常用
    BUTTON_EVENT_SHORT_START,  // 短按开始事件
    BUTTON_EVENT_SHORT_UP,     // 短按抬起事件，常用
    BUTTON_EVENT_LONG_START,   // 长按开始事件
    BUTTON_EVENT_LONG_UP,      // 长按抬起事件，常用
    BUTTON_EVENT_LONG_HOLD,    // 长按保持事件
    BUTTON_EVENT_LONG_HOLD_UP, // 长按保持的抬起事件，常用于累加
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_MAX,
} button_event_t;

typedef struct button_t
{
    uint8_t ticks;                   /* 节拍数 */
    uint8_t clicks;                  /* 连击数 */
    uint8_t pressed_logic_level : 1; /* 按下电平 */
    uint8_t status : 3;              /* 状态 */
    uint8_t event : 4;               /* 事件 */
} button_t;

extern button_t button[BUTTON_MAX];

void button_init(void);
void button_scan(void);

#endif /* __BUTTON_H__ */
