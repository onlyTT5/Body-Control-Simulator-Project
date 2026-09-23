#include "bsp_button.h"

static GPIO_PinState last_light_button_state;

void BspButton_Init(void)
{
    last_light_button_state = HAL_GPIO_ReadPin(
        BTN_LIGHT_GPIO_Port,
        BTN_LIGHT_Pin
    );
}

uint8_t BspButton_LightWasPressed(void)
{
    GPIO_PinState current_state;
    uint8_t pressed = 0U;

    current_state = HAL_GPIO_ReadPin(
        BTN_LIGHT_GPIO_Port,
        BTN_LIGHT_Pin
    );

    if ((last_light_button_state == GPIO_PIN_SET) &&
        (current_state == GPIO_PIN_RESET))
    {
        HAL_Delay(20);

        if (HAL_GPIO_ReadPin(
                BTN_LIGHT_GPIO_Port,
                BTN_LIGHT_Pin
            ) == GPIO_PIN_RESET)
        {
            pressed = 1U;
        }
    }

    last_light_button_state = current_state;

    return pressed;
}
