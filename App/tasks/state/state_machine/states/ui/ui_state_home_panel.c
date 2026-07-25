#include "ui_state_home_panel.h"

#include "ui_panel_id.h"
#include "ui_state_setting_panel.h"

const State UI_STATE_HOME_PANEL = {
    .on_enter = UiStateHomePanel_OnEnter,
    .on_event = UiStateHomePanel_OnEvent,
    .on_exit =  UiStateHomePanel_OnExit,
    .id = UI_PANEL_ID_HOME,
};

void UiStateHomePanel_OnEnter(void *context) {
}

StateOnEventHandlingResult UiStateHomePanel_OnEvent(const StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        ControlButtonPayload button_payload = state_event->payload.control_button;
        if (button_payload.id == CONTROL_BUTTON_ID_LEFT) {
            // 실제 트랜지션은 상태 관리 태스크에서 수행하기
            return (StateOnEventHandlingResult) {
                .next_state = &UI_STATE_SETTING_PANEL,
                .status = STATE_ON_EVENT_HANDLING_STATUS_TRANSITION
            };
        }
    }

    return (StateOnEventHandlingResult){
        .next_state = 0,
        .status = STATE_ON_EVENT_HANDLING_STATUS_HANDLED
    };
}

void UiStateHomePanel_OnExit(void) {

}
