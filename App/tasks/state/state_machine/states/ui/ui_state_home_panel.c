#include "ui_state_home_panel.h"

#include "cmsis_os2.h"
#include "display_messages.h"
#include "ui_panel_id.h"
#include "ui_state.h"
#include "ui_state_setting_panel.h"
#include "loopstation_parameter_store.h"

const State UI_STATE_HOME_PANEL = {
    .on_enter = UiStateHomePanel_OnEnter,
    .on_event = UiStateHomePanel_OnEvent,
    .id = UI_PANEL_ID_HOME,
};

StateFunctionStatus UiStateHomePanel_OnEnter(void *context)
{
    UiStateMachineContext *ui_state_machine_context = context;
    // TODO:
    // 디스플레이 태스크에게 요청하는 코드 구현하기
}

StateFunctionStatus UiStateHomePanel_OnEvent(StateEvent *state_event, void *state_machine_context)
{}
