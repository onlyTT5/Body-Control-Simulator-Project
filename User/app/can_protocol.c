#include "can_protocol.h"
#include <stddef.h>

void CanProtocol_BuildLightControl(CanProtocolFrame *frame,
                                    uint8_t light_on)
{
    uint8_t i;

    if (frame == NULL)
    {
        return;
    }

    frame->std_id = CAN_ID_LIGHT_CONTROL;
    frame->dlc = CAN_PROTOCOL_DLC;

    for (i = 0U; i < CAN_PROTOCOL_DLC; i++)
    {
        frame->data[i] = 0U;
    }

    if (light_on)
    {
        frame->data[0] |= CAN_LIGHT_ON_MASK;
    }
}

uint8_t CanProtocol_ParseLightControl(const CanProtocolFrame *frame,
                                       uint8_t *light_on)
{
    if ((frame == NULL) || (light_on == NULL))
    {
        return 0U;
    }

    if ((frame->std_id != CAN_ID_LIGHT_CONTROL) ||
        (frame->dlc != CAN_PROTOCOL_DLC))
    {
        return 0U;
    }

    *light_on = (frame->data[0] & CAN_LIGHT_ON_MASK) ? 1U : 0U;

    return 1U;
}
