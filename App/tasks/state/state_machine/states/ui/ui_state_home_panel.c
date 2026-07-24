#include "ui_state_home_panel.h"

const State UI_STATE_HOME_PANEL = {
    .on_enter = UiStateHomePanel_OnEnter,
    .on_event = UiStateHomePanel_OnEvent,
    .on_exit =  UiStateHomePanel_OnExit,
};

void UiStateHomePanel_OnEnter(void *context) {
}

EventHandlingResult UiStateHomePanel_OnEvent(const StateEvent *state_event)
{
}

void UiStateHomePanel_OnExit(void) {

}
