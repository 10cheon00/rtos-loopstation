#ifndef HOME_PANEL_H
#define HOME_PANEL_H

#include "state.h"

void DisplayHomePanel_OnEnter(void *context);
EventHandlingResult DisplayHomePanel_OnEvent(const StateEvent *state_event);
void DisplayHomePanel_OnExit(void);

extern const State DISPLAY_STATE_HOME_PANEL;

#endif
