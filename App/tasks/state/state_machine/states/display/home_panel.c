#include "home_panel.h"
#include "setting_panel.h"
#include "display_context.h"
#include "display_task.h"

void DisplayHomePanel_OnEnter(void *context) {
    DisplayPanelContext* display_context = context;
}

EventHandlingResult DisplayHomePanel_OnEvent(const StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        ControlButtonPayload button_payload = state_event->payload.control_button;
        if (button_payload.id == CONTROL_BUTTON_ID_LEFT) {
            // 실제 트랜지션은 상태 관리 태스크에서 수행하기
            return (EventHandlingResult) {
                .next_state = &DISPLAY_STATE_SETTING_PANEL,
                .status = EVENT_HANDLING_STATUS_TRANSITION
            };
        }
    }

    return (EventHandlingResult){ 
        .next_state = 0,
        .status = EVENT_HANDLING_STATUS_HANDLED
    };
}

void DisplayHomePanel_OnExit(void) {

}
