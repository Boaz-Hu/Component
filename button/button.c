#include "button.h"
#include "stm32f1xx_hal.h"

/* 读取按键电平 */
#define BUTTON_READ_LEVEL(level, id) (((level == button[id].pressed_logic_level) ? 1 : 0) << id)

enum BUTTON_STAGE
{
    BUTTON_STAGE_DEFAULT = 0,
    BUTTON_STAGE_DOWN = 1,
    BUTTON_STAGE_MULTIPLE_CLICK = 2
};

/* 按键 */
button_t button[BUTTON_MAX];

/**
 * @brief 按键初始化
 * 
 * @note 用户提供I/O初始化操作
 */
void button_init(void)
{
    uint8_t i;

    /* -------------------------- I/O初始化 -------------------------- */
    // RCC->APB2ENR |= 1 << 2; //开启PA时钟
    // RCC->APB2ENR |= 1 << 4; //开启PC时钟

    // JTAG_Set(SWD_ENABLE);
    // GPIOA->CRL &= 0xFFFFFFF0; //PA0下拉输入
    // GPIOA->CRL |= 0x00000008;
    // GPIOA->ODR |= 0;
    // GPIOA->CRH &= 0x0FFFFFFF; //PA15上拉输入
    // GPIOA->CRH |= 0x80000000;
    // GPIOA->ODR |= 1 << 15;
    // GPIOC->CRL &= 0xFF0FFFFF; //PC5上拉输入
    // GPIOC->CRL |= 0x00800000;
    // GPIOC->ODR |= 1 << 5;
    /* ------------------------------------------------------------------------ */

    for (i = 0; i < BUTTON_MAX; i++)
    {
        button[i].status = BUTTON_STAGE_DEFAULT;
        button[i].event = BUTTON_EVENT_NONE;
        /* -------------------------- 用户指定按键按下电平 -------------------------- */
        button[i].pressed_logic_level = 0;
        if (i == BUTTON_WK_UP)
        {
            button[i].pressed_logic_level = 1;
        }
        /* ------------------------------------------------------------------------ */
    }
}

/**
 * @brief 按键扫描
 * 
 * @note 用户提供读取I/O操作
 */
void button_scan(void)
{
    uint8_t i = 0;
    uint8_t button_is_pressed = 0;
    uint8_t button_logic_level = 0;

    /* -------------------------- 用户提供读取I/O操作 -------------------------- */
    button_logic_level = BUTTON_READ_LEVEL(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0), BUTTON_WK_UP) | 
                         BUTTON_READ_LEVEL(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5), BUTTON_KEY0)  | 
                         BUTTON_READ_LEVEL(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15), BUTTON_KEY1);
    /* ------------------------------------------------------------------------ */

    for (i = 0; i < BUTTON_MAX; i++)
    {
        button_is_pressed = button_logic_level & (1 << i);

        if (button[i].status > BUTTON_STAGE_DEFAULT)
        {
            button[i].ticks++;
            if (button[i].ticks >= BUTTON_LONG_HOLD_START_TICK)
            {
                button[i].ticks = BUTTON_LONG_HOLD_START_TICK;
            }
        }

        switch (button[i].status)
        {
        case BUTTON_STAGE_DEFAULT: /* stage: default(button up) */
            button[i].event = BUTTON_EVENT_NONE;
            if (button_is_pressed) /* is pressed */
            {
                button[i].event = BUTTON_EVENT_DOWN;
                button[i].status = BUTTON_STAGE_DOWN;
                button[i].ticks = 0;
                button[i].clicks = 0;
            }
            break;
        case BUTTON_STAGE_DOWN:    /* stage: button down */
            if (button_is_pressed) /* is pressed */
            {
                if (button[i].clicks > 0) /* multiple click */
                {
                    if (button[i].ticks > MAX_MULTIPLE_CLICKS_INTERVAL)
                    {
                        button[i].event =
                            button[i].clicks < BUTTON_EVENT_REPEAT_CLICK ? button[i].clicks : BUTTON_EVENT_REPEAT_CLICK;

                        button[i].status = BUTTON_STAGE_DOWN;
                        button[i].ticks = 0;
                        button[i].clicks = 0;
                    }
                }
                else if (button[i].ticks >= BUTTON_LONG_HOLD_START_TICK)
                {
                    if (button[i].event != BUTTON_EVENT_LONG_HOLD)
                    {
                        button[i].event = BUTTON_EVENT_LONG_HOLD;
                    }
                }
                else if (button[i].ticks >= BUTTON_LONG_PRESS_START_TICK)
                {
                    if (button[i].event != BUTTON_EVENT_LONG_START)
                    {
                        button[i].event = BUTTON_EVENT_LONG_START;
                    }
                }
                else if (button[i].ticks >= BUTTON_SHORT_PRESS_START_TICK)
                {
                    if (button[i].event != BUTTON_EVENT_SHORT_START)
                    {
                        button[i].event = BUTTON_EVENT_SHORT_START;
                    }
                }
            }
            else /* button up */
            {
                if (button[i].ticks >= BUTTON_LONG_HOLD_START_TICK)
                {
                    button[i].event = BUTTON_EVENT_LONG_HOLD_UP;
                    button[i].status = BUTTON_STAGE_DEFAULT;
                }
                else if (button[i].ticks >= BUTTON_LONG_PRESS_START_TICK)
                {
                    button[i].event = BUTTON_EVENT_LONG_UP;
                    button[i].status = BUTTON_STAGE_DEFAULT;
                }
                else if (button[i].ticks >= BUTTON_SHORT_PRESS_START_TICK)
                {
                    button[i].event = BUTTON_EVENT_SHORT_UP;
                    button[i].status = BUTTON_STAGE_DEFAULT;
                }
                else
                {
                    /* swtich to multiple click stage */
                    button[i].status = BUTTON_STAGE_MULTIPLE_CLICK;
                    button[i].clicks++;
                }
            }
            break;
        case BUTTON_STAGE_MULTIPLE_CLICK:      /* stage: multiple click */
            if (button_is_pressed)             /* is pressed */
            {
                /* swtich to button down stage */
                button[i].status = BUTTON_STAGE_DOWN;
                button[i].ticks = 0;
            }
            else
            {
                if (button[i].ticks > MAX_MULTIPLE_CLICKS_INTERVAL)
                {
                    button[i].event =
                        button[i].clicks < BUTTON_EVENT_REPEAT_CLICK ? button[i].clicks : BUTTON_EVENT_REPEAT_CLICK;

                    /* swtich to default stage */
                    button[i].status = BUTTON_STAGE_DEFAULT;
                }
            }
            break;
        }
    }
}
