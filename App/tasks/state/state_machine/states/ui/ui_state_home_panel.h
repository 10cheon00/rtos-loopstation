#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "state.h"

void UiStateHomePanel_OnEnter(void *context);
EventHandlingResult UiStateHomePanel_OnEvent(const StateEvent *state_event);
void UiStateHomePanel_OnExit(void);

extern const State UI_STATE_HOME_PANEL;

#endif
