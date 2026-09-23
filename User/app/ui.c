#include "ui.h"
#include "oled.h"

void Ui_ShowStatus(const BodyControlState *state)
{
    OLED_Clear();

    OLED_ShowString(0, 10, "BODY CONTROL V0.1");
    OLED_ShowString(2, 20, "CAN: OFFLINE");

    if (state->light_on)
    {
        OLED_ShowString(4, 20, "LIGHT: ON");
    }
    else
    {
        OLED_ShowString(4, 20, "LIGHT: OFF");
    }
}
