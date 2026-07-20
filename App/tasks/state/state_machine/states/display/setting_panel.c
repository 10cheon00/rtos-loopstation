#include "setting_panel.h"

#include "FreeRTOS.h"

#include "home_panel.h"
#include "display_messages.h"
#include "display_context.h"

#define SETTING_PANEL_TIMEOUT_MS 500
#define SETTING_PANEL_TIMEOUT_TICKS (pdMS_TO_TICKS(SETTING_PANEL_TIMEOUT_MS))

const State DISPLAY_STATE_SETTING_PANEL = {
    .on_enter = DisplaySettingPanel_OnEnter,
    .on_event = DisplaySettingPanel_OnEvent,
    .on_exit = DisplaySettingPanel_OnExit,
};

void DisplaySettingPanel_OnEnter(void *context) {
    DisplayPanelContext* display_context = context;
    DisplayCommand command = {
        .type = DISPLAY_COMMAND_UI_STATE_RENDER,
        .payload = {
            .ui_state_render = {
                .panel_id = 2
            }
        }
    };
    osMessageQueuePut(display_context->display_command_queue,
        &command, 0, SETTING_PANEL_TIMEOUT_TICKS);
}

EventHandlingResult DisplaySettingPanel_OnEvent(const StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        ControlButtonPayload button_payload = state_event->payload.control_button;
        if (button_payload.id == CONTROL_BUTTON_ID_RIGHT) {
            // 실제 트랜지션은 상태 관리 태스크에서 수행하기
            return (EventHandlingResult) {
                .next_state = &DISPLAY_STATE_HOME_PANEL,
                .status = EVENT_HANDLING_STATUS_TRANSITION
            };
        }
    }
    return (EventHandlingResult){ 
        .next_state = 0,
        .status = EVENT_HANDLING_STATUS_HANDLED
    };
}

void DisplaySettingPanel_OnExit(void) {

}
