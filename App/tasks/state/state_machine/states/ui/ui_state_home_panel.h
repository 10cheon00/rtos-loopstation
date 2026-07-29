#ifndef UI_STATE_HOME_PANEL_H
#define UI_STATE_HOME_PANEL_H

#include "state.h"

StateOnEventResultFlags UiStateHomePanel_OnEvent(const StateEvent *state_event,
                                                 StateId *next_state_id);

extern const State UI_STATE_HOME_PANEL;

#endif
