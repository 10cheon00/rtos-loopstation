#ifndef UI_STATE_SETTING_PANEL_H
#define UI_STATE_SETTING_PANEL_H

#include "state.h"

void UiStateSettingPanel_OnEnter(void *context);
StateOnEventHandlingResult UiStateSettingPanel_OnEvent(const StateEvent *state_event);
void UiStateSettingPanel_OnExit(void);

extern const State UI_STATE_SETTING_PANEL;

#endif
