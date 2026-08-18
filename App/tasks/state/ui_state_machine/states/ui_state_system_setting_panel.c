#include "ui_state_system_setting_panel.h"

#include "utils.h"

UiTransitionMapEntry ui_system_setting_panel_transition_map[] = {
    {.ui_action_id = UI_ACTION_ID_EXIT, .next_ui_panel_id = UI_PANEL_ID_SETTING},
};

UiState UI_STATE_SYSTEM_SETTING_PANEL = {
    .ui_transition_map = ui_system_setting_panel_transition_map,
    .ui_transition_map_count = ARRAY_COUNT(ui_system_setting_panel_transition_map),
    .ui_panel_id = UI_PANEL_ID_SYSTEM_SETTING};
