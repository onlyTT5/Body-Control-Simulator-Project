#include "stm32f10x.h"
#include "bsp_can.h"
#include "bsp_tick.h"

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

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

    if(BspCan_Init() != 1U)
    {
        while(1)
        {
        }
    }

    last_heartbeat_tick = BspTick_GetMs();

    while(1)
    {
        /*
         * 每 1000 ms 发送一次心跳帧。
         * data[0] 放入递增序号，便于在 USB-CAN 软件中确认它持续在发送。
         */
        if((BspTick_GetMs() - last_heartbeat_tick) >= 1000U)
        {
            heartbeat_data[0] = heartbeat_sequence;

            (void)BspCan_SendStdData(0x101U,
                                     heartbeat_data,
                                     8U);

            heartbeat_sequence++;
            last_heartbeat_tick = BspTick_GetMs();
        }

        /* 接收节点 A 或 USB-CAN 发来的灯光控制帧 */
        if(BspCan_ReceiveStdData(&rx_id, rx_data, &rx_dlc) == 1U)
        {
            if((rx_id == 0x100U) && (rx_dlc == 8U))
            {
                if((rx_data[0] & 0x01U) != 0U)
                {
                    GPIO_ResetBits(GPIOC, GPIO_Pin_13); /* 开灯 */
                }
                else
                {
                    GPIO_SetBits(GPIOC, GPIO_Pin_13);   /* 关灯 */
                }
            }
        }
    }
}