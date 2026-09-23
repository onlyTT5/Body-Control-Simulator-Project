#include "app_main.h"

#include "bsp_button.h"
#include "bsp_led.h"
#include "body_control.h"
#include "ui.h"
#include "can_protocol.h"

static void AppMain_FlashLed(uint8_t count, uint32_t delay_ms)
{
    uint8_t i;

    for (i = 0U; i < count; i++)
    {
        BspLed_Set(1U);
        HAL_Delay(delay_ms);

        BspLed_Set(0U);
        HAL_Delay(delay_ms);
    }
}

static uint8_t AppMain_CanProtocolSelfTest(void)
{
    CanProtocolFrame frame;
    uint8_t light_on;

    /* 测试 1：灯光开启帧的构造与解析 */
    CanProtocol_BuildLightControl(&frame, 1U);

    if ((frame.std_id != CAN_ID_LIGHT_CONTROL) ||
        (frame.dlc != CAN_PROTOCOL_DLC) ||
        (frame.data[0] != CAN_LIGHT_ON_MASK))
    {
        return 0U;
    }

    light_on = 0U;

    if (CanProtocol_ParseLightControl(&frame, &light_on) != 1U)
    {
        return 0U;
    }

    if (light_on != 1U)
    {
        return 0U;
    }

    /* 测试 2：灯光关闭帧的构造与解析 */
    CanProtocol_BuildLightControl(&frame, 0U);
    light_on = 1U;

    if (CanProtocol_ParseLightControl(&frame, &light_on) != 1U)
    {
        return 0U;
    }

    if (light_on != 0U)
    {
        return 0U;
    }

    /* 测试 3：错误 ID 必须被拒绝 */
    frame.std_id = 0x101U;

    if (CanProtocol_ParseLightControl(&frame, &light_on) != 0U)
    {
        return 0U;
    }

    /* 测试 4：错误 DLC 必须被拒绝 */
    frame.std_id = CAN_ID_LIGHT_CONTROL;
    frame.dlc = 7U;

    if (CanProtocol_ParseLightControl(&frame, &light_on) != 0U)
    {
        return 0U;
    }

    return 1U;
}

void AppMain_Init(void)
{
    BspButton_Init();
    BodyControl_Init();

    Ui_ShowBootSelfTest();

    /* 板载 LED 自检 */
		AppMain_FlashLed(3U, 150U);

		/* CAN 协议软件自测：通过快闪两次；失败则常亮 1 秒 */
		if (AppMain_CanProtocolSelfTest())
		{
				AppMain_FlashLed(2U, 80U);
		}
		else
		{
				BspLed_Set(1U);
				HAL_Delay(1000U);
				BspLed_Set(0U);
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
