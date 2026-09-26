#include "stm32f10x.h"
#include "bsp_can.h"
#include "bsp_tick.h"
#include "bsp_buzzer.h"

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    uint8_t light_on = 0U;
    uint8_t requested_light_on;

    uint8_t buzzer_on = 0U;
    uint32_t buzzer_start_tick = 0U;

    uint32_t current_tick;

    uint8_t heartbeat_data[8] = {0U};
    uint8_t heartbeat_sequence = 0U;

    uint16_t rx_id;
    uint8_t rx_data[8];
    uint8_t rx_dlc;

    uint32_t last_heartbeat_tick;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 默认熄灭：PC13 为低电平点亮 */
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    BspTick_Init();

    BspBuzzer_Init();

    if(BspCan_Init() != 1U)
    {
        while(1)
        {
        }
    }

    last_heartbeat_tick = BspTick_GetMs();

    while(1)
    {
        current_tick = BspTick_GetMs();

        /* 每秒发送一次节点 B 心跳 */
        if((current_tick - last_heartbeat_tick) >= 1000U)
        {
            heartbeat_data[0] = heartbeat_sequence;

            (void)BspCan_SendStdData(0x101U,
                                     heartbeat_data,
                                     8U);

            heartbeat_sequence++;
            last_heartbeat_tick = current_tick;
        }

        /* 接收并执行节点 A 发来的灯光命令 */
        if(BspCan_ReceiveStdData(&rx_id, rx_data, &rx_dlc) == 1U)
        {
            if((rx_id == 0x100U) && (rx_dlc == 8U))
            {
                requested_light_on = ((rx_data[0] & 0x01U) != 0U) ? 1U : 0U;

                /* 仅在状态改变时执行并鸣叫 */
                if(requested_light_on != light_on)
                {
                    light_on = requested_light_on;

                    if(light_on != 0U)
                    {
                        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
                    }
                    else
                    {
                        GPIO_SetBits(GPIOC, GPIO_Pin_13);
                    }

                    BspBuzzer_Set(1U);
                    buzzer_on = 1U;
                    buzzer_start_tick = current_tick;
                }
            }
        }

        /* 到 100 ms 自动关闭蜂鸣器；不使用阻塞延时 */
        if((buzzer_on != 0U) &&
                ((current_tick - buzzer_start_tick) >= 100U))
        {
            BspBuzzer_Set(0U);
            buzzer_on = 0U;
        }
    }
}
