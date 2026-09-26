#include "bsp_can.h"

HAL_StatusTypeDef BspCan_Init(void)
{
    CAN_FilterTypeDef filter_config;

    /* 当前接收全部 CAN 报文；后续可改成只接收指定 ID */
    filter_config.FilterIdHigh = 0x0000U;
    filter_config.FilterIdLow = 0x0000U;
    filter_config.FilterMaskIdHigh = 0x0000U;
    filter_config.FilterMaskIdLow = 0x0000U;

    filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter_config.FilterBank = 0U;
    filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
    filter_config.FilterActivation = ENABLE;
    filter_config.SlaveStartFilterBank = 14U;

    if (HAL_CAN_ConfigFilter(&hcan, &filter_config) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef BspCan_SendStdData(uint16_t standard_id,
                                     const uint8_t *data,
                                     uint8_t dlc)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t mailbox;
    uint8_t tx_data[BSP_CAN_MAX_DLC];
    uint8_t i;

    if ((data == NULL) || (dlc > BSP_CAN_MAX_DLC))
    {
        return HAL_ERROR;
    }

    for (i = 0U; i < dlc; i++)
    {
        tx_data[i] = data[i];
    }

    tx_header.StdId = standard_id;
    tx_header.ExtId = 0U;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan, &tx_header, tx_data, &mailbox);
}

uint8_t BspCan_ReceiveStdData(uint16_t *standard_id,
                              uint8_t *data,
                              uint8_t *dlc)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[BSP_CAN_MAX_DLC];
    uint8_t i;

    if ((standard_id == NULL) || (data == NULL) || (dlc == NULL))
    {
        return 0U;
    }

    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0U)
    {
        return 0U;
    }

    if (HAL_CAN_GetRxMessage(&hcan,
                             CAN_RX_FIFO0,
                             &rx_header,
                             rx_data) != HAL_OK)
    {
        return 0U;
    }

    /* 当前项目只处理 11 位标准数据帧 */
    if ((rx_header.IDE != CAN_ID_STD) ||
        (rx_header.RTR != CAN_RTR_DATA) ||
        (rx_header.DLC > BSP_CAN_MAX_DLC))
    {
        return 0U;
    }

    *standard_id = (uint16_t)rx_header.StdId;
    *dlc = (uint8_t)rx_header.DLC;

    for (i = 0U; i < *dlc; i++)
    {
        data[i] = rx_data[i];
    }

    return 1U;
}
