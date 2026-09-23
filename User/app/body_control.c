#include "body_control.h"
#include "bsp_led.h"

static BodyControlState state;

void BodyControl_Init(void)
{
    state.light_on = 0U;
    state.can_online = 0U;

    BspLed_Set(0U);
}

void BodyControl_ToggleLight(void)
{
    state.light_on = !state.light_on;

    BspLed_Set(state.light_on);
}

const BodyControlState *BodyControl_GetState(void)
{
    return &state;
}
