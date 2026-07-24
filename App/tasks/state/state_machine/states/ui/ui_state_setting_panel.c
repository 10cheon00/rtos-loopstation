#include "ui_state_setting_panel.h"

#include "ui_state_home_panel.h"
#include "ui_panel_id.h"

const State UI_STATE_SETTING_PANEL = {
    .on_enter = UiStateSettingPanel_OnEnter,
    .on_event = UiStateSettingPanel_OnEvent,
    .on_exit = UiStateSettingPanel_OnExit,
    .id = UI_PANEL_ID_SETTING,
};

void UiStateSettingPanel_OnEnter(void *context) {
}

EventHandlingResult UiStateSettingPanel_OnEvent(const StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        ControlButtonPayload button_payload = state_event->payload.control_button;
        if (button_payload.id == CONTROL_BUTTON_ID_RIGHT) {
            // 실제 트랜지션은 상태 관리 태스크에서 수행하기
            return (EventHandlingResult) {
                .next_state = &UI_STATE_HOME_PANEL,
                .status = EVENT_HANDLING_STATUS_TRANSITION
            };
        }
    }

    return (EventHandlingResult){ 
        .next_state = 0,
        .status = EVENT_HANDLING_STATUS_HANDLED
    };
}

void UiStateSettingPanel_OnExit(void) {

}
