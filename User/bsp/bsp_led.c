#include "bsp_led.h"
#include "main.h"

void BspLed_Set(uint8_t on)
{
    HAL_GPIO_WritePin(
        STATUS_LED_GPIO_Port,
        STATUS_LED_Pin,
        on ? GPIO_PIN_RESET : GPIO_PIN_SET
    );
}
