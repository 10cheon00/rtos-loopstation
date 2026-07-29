#include "ui_state_home_panel.h"

UiState UI_STATE_HOME_PANEL = {.UiState_OnEnter = UiStateHomePanel_OnEnter,
                               .UiState_OnEvent = UiStateHomePanel_OnEvent,
                               .ui_panel_id = UI_PANEL_ID_HOME};

void UiStateHomePanel_OnEnter(UiStateMachine *ui_state_machine) {}

void UiStateHomePanel_OnEvent(UiStateMachine *ui_state_machine, UiStateEventId ui_state_event_id) {}
