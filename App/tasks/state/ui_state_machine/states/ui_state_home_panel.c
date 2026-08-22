#include "ui_state_home_panel.h"

#include "utils.h"

UiTransitionMapEntry ui_home_panel_transition_mappings[] = {
    {.ui_action_id = UI_ACTION_ID_NAVIGATE_LEFT, .next_ui_state_id = UI_STATE_ID_SETTING},
};

UiState UI_STATE_HOME_PANEL = {.ui_transition_map = ui_home_panel_transition_mappings,
                               .ui_transition_map_count =
                                   ARRAY_COUNT(ui_home_panel_transition_mappings),
                               .ui_state_id = UI_STATE_ID_HOME};
