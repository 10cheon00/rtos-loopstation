#include "ui_state_home_panel.h"

#include "utils.h"

UiTransitionMapping ui_home_panel_transition_mappings[] = {
    {.ui_action_id = UI_ACTION_NAVIGATE_LEFT, .next_ui_panel_id = UI_PANEL_ID_SETTING},
};

UiState UI_STATE_HOME_PANEL = {.ui_transition_mappings = ui_home_panel_transition_mappings,
                               .ui_transition_mappings_count =
                                   ARRAY_COUNT(ui_home_panel_transition_mappings),
                               .ui_panel_id = UI_PANEL_ID_HOME};
