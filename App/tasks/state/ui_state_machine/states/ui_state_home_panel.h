#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "ui_state.h"

extern UiState UI_STATE_HOME_PANEL;

void UiStateHomePanel_OnEnter(UiStateMachine* ui_state_machine);
void UiStateHomePanel_OnEvent(UiStateMachine* ui_state_machine, UiStateEventId ui_state_event_id);

#endif