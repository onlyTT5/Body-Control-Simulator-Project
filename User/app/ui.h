#ifndef UI_H
#define UI_H

#include "body_control.h"

void Ui_ShowBootSelfTest(void);

void Ui_InitDashboard(void);
void Ui_ShowStatus(const BodyControlState *state);
void Ui_UpdateLight(const BodyControlState *state);
void Ui_UpdateHeartbeat(const BodyControlState *state);

#endif
