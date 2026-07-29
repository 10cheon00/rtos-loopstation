#ifndef UI_STATE_SETTING_PANEL_H
#define UI_STATE_SETTING_PANEL_H

#include "state.h"

void UiStateSettingPanel_OnEnter();
StateOnEventResultFlags UiStateSettingPanel_OnEvent(const StateEvent *state_event,
                                                    StateId *next_state_id);
void UiStateSettingPanel_OnExit(void);

extern const State UI_STATE_SETTING_PANEL;

#endif
