#include "ui_state_setting_panel.h"

#include "utils.h"

UiTransitionMapping ui_setting_panel_transition_mappings[] = {
    {.ui_action_id = UI_ACTION_NAVIGATE_RIGHT, .next_ui_panel_id = UI_PANEL_ID_HOME},
};

UiState UI_STATE_SETTING_PANEL = {.ui_transition_mappings = ui_setting_panel_transition_mappings,
                                  .ui_transition_mappings_count =
                                      ARRAY_COUNT(ui_setting_panel_transition_mappings),
                                  .ui_panel_id = UI_PANEL_ID_HOME};
