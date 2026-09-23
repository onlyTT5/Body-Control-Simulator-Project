#include "bsp_button.h"

#define BUTTON_DEBOUNCE_MS  20U

static GPIO_PinState last_raw_state;
static GPIO_PinState stable_state;
static uint32_t last_change_tick;

void BspButton_Init(void)
{
    last_raw_state = HAL_GPIO_ReadPin(BTN_LIGHT_GPIO_Port, BTN_LIGHT_Pin);
    stable_state = last_raw_state;
    last_change_tick = HAL_GetTick();
}

uint8_t BspButton_LightWasPressed(void)
{
    GPIO_PinState current_raw_state;
    uint32_t current_tick;

    current_raw_state = HAL_GPIO_ReadPin(BTN_LIGHT_GPIO_Port, BTN_LIGHT_Pin);
    current_tick = HAL_GetTick();

    /* 原始电平发生变化：重新开始计算消抖时间 */
    if (current_raw_state != last_raw_state)
    {
        last_raw_state = current_raw_state;
        last_change_tick = current_tick;
    }

    /* 电平稳定超过 20ms，才认定为一次有效状态变化 */
    if ((current_tick - last_change_tick >= BUTTON_DEBOUNCE_MS) &&
        (stable_state != last_raw_state))
    {
        stable_state = last_raw_state;

        /* PB11 使用上拉输入：低电平代表按下 */
        if (stable_state == GPIO_PIN_RESET)
        {
            return 1U;
        }
    }

    return 0U;
}
