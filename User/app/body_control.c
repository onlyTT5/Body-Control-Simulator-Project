#include "body_control.h"
#include "bsp_led.h"

#define HEARTBEAT_PERIOD_MS  1000U

static BodyControlState state;
static uint32_t last_heartbeat_tick;

void BodyControl_Init(void)
{
    state.light_on = 0U;
    state.can_online = 0U;
    state.heartbeat_on = 0U;
    state.heartbeat_count = 0U;

    last_heartbeat_tick = HAL_GetTick();

    BspLed_Set(0U);
}

void BodyControl_ToggleLight(void)
{
    state.light_on = !state.light_on;
    BspLed_Set(state.light_on);
}

uint8_t BodyControl_HeartbeatTask(void)
{
    uint32_t current_tick = HAL_GetTick();

    if (current_tick - last_heartbeat_tick >= HEARTBEAT_PERIOD_MS)
    {
        last_heartbeat_tick = current_tick;

        state.heartbeat_on = !state.heartbeat_on;
        state.heartbeat_count++;

        return 1U;
    }

    return 0U;
}

const BodyControlState *BodyControl_GetState(void)
{
    return &state;
}
