#include "app_main.h"

#include "bsp_button.h"
#include "bsp_led.h"
#include "body_control.h"
#include "ui.h"

void AppMain_Init(void)
{
    BspButton_Init();
    BodyControl_Init();

    Ui_ShowBootSelfTest();

    /* 板载 LED 开机自检 */
    for (uint8_t i = 0U; i < 3U; i++)
    {
        BspLed_Set(1U);
        HAL_Delay(150U);

        BspLed_Set(0U);
        HAL_Delay(150U);
    }

    HAL_Delay(800U);

    Ui_InitDashboard();
    Ui_ShowStatus(BodyControl_GetState());
}

void AppMain_Run(void)
{
    if (BspButton_LightWasPressed())
    {
        BodyControl_ToggleLight();
        Ui_UpdateLight(BodyControl_GetState());
    }

    if (BodyControl_HeartbeatTask())
    {
        Ui_UpdateHeartbeat(BodyControl_GetState());
    }
}