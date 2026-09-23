#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "can.h"

#define BSP_CAN_MAX_DLC  8U

HAL_StatusTypeDef BspCan_Init(void);

HAL_StatusTypeDef BspCan_SendStdData(uint16_t standard_id,
                                     const uint8_t *data,
                                     uint8_t dlc);

uint8_t BspCan_ReceiveStdData(uint16_t *standard_id,
                              uint8_t *data,
                              uint8_t *dlc);

#endif
																		 