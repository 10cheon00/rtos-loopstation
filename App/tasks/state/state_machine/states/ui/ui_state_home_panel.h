#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "state.h"

StateFunctionStatus UiStateHomePanel_OnEnter(void *state_machine_context);
StateFunctionStatus UiStateHomePanel_OnEvent(StateEvent *state_event, void *state_machine_context);

extern const State UI_STATE_HOME_PANEL;

#endif
