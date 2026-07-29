#ifndef UI_STATE_SETTING_PANEL_H
#define UI_STATE_SETTING_PANEL_H

#include "state.h"

StateFunctionStatus UiStateSettingPanel_OnEnter(void *state_machine_context);
StateFunctionStatus UiStateSettingPanel_OnEvent(StateEvent *state_event, void *state_machine_context);

extern const State UI_STATE_SETTING_PANEL;

#endif
