#ifndef BODY_CONTROL_H
#define BODY_CONTROL_H

#include "main.h"

typedef struct
{
    uint8_t light_on;
    uint8_t can_online;

    uint8_t heartbeat_on;
    uint32_t heartbeat_count;
} BodyControlState;

void BodyControl_Init(void);
void BodyControl_ToggleLight(void);

/* 每次主循环调用；仅在满 1 秒并更新心跳时返回 1U */
uint8_t BodyControl_HeartbeatTask(void);

const BodyControlState *BodyControl_GetState(void);

#endif