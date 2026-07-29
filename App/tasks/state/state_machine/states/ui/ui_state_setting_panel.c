#include "ui_state_setting_panel.h"

#include "ui_state_home_panel.h"
#include "ui_panel_id.h"

const State UI_STATE_SETTING_PANEL = {
    .on_enter = UiStateSettingPanel_OnEnter,
    .on_event = UiStateSettingPanel_OnEvent,
    .on_exit = UiStateSettingPanel_OnExit,
    .id = UI_PANEL_ID_SETTING,
};

void UiStateSettingPanel_OnEnter() {}

StateOnEventResultFlags UiStateSettingPanel_OnEvent(const StateEvent *state_event,
                                                    StateId *next_state_id)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        ControlButtonPayload button_payload = state_event->payload.control_button;
        if (button_payload.id == CONTROL_BUTTON_ID_RIGHT) {
            // 실제 트랜지션은 상태 관리 태스크에서 수행하기
            // TODO: 
            // 다음 상태를 가리키는 것을 더블 포인터로 구현할 수 있으나,
            // 그러면 복잡해지므로, 더블 포인터 대신 패널 id로 한다.
            // State와 패널 id간 매핑 관계를 정의한 파일이 필요함
            *next_state_id = UI_PANEL_ID_HOME;
            return STATE_ON_EVENT_HANDLING_FLAG_TRANSITION;
        }
    }

    return STATE_ON_EVENT_HANDLING_FLAG_IGNORED;
}

void UiStateSettingPanel_OnExit(void) {}
