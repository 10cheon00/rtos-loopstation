#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H

#include "state.h"

void DisplaySettingPanel_OnEnter(void *context);
EventHandlingResult DisplaySettingPanel_OnEvent(const StateEvent *state_event);
void DisplaySettingPanel_OnExit(void);

extern const State DISPLAY_STATE_SETTING_PANEL;

#endif
