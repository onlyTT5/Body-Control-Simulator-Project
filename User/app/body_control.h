#ifndef BODY_CONTROL_H
#define BODY_CONTROL_H

#include "main.h"

typedef struct
{
    uint8_t light_on;
    uint8_t can_online;
} BodyControlState;

void BodyControl_Init(void);
void BodyControl_ToggleLight(void);
const BodyControlState *BodyControl_GetState(void);

#endif
