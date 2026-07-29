#include "ui_state_setting_panel.h"

#include "ui_state_home_panel.h"
#include "ui_panel_id.h"

const State UI_STATE_SETTING_PANEL = {
    .on_enter = UiStateHomePanel_OnEnter,
    .on_event = UiStateSettingPanel_OnEvent,
    .id = UI_PANEL_ID_SETTING,
};

StateFunctionStatus UiStateSettingPanel_OnEnter(void *state_machine_context)
{
    return STATE_FUNCTION_STATUS_OK;
}

StateFunctionStatus UiStateSettingPanel_OnEvent(StateEvent *state_event,
                                                void *state_machine_context)
{
    return STATE_FUNCTION_STATUS_OK;
}
