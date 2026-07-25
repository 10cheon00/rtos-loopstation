#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "state.h"

void UiStateHomePanel_OnEnter(void *context);
StateOnEventResultFlags UiStateHomePanel_OnEvent(const StateEvent *state_event,
                                                 StateId *next_state_id);
void UiStateHomePanel_OnExit(void);

extern const State UI_STATE_HOME_PANEL;

#endif
