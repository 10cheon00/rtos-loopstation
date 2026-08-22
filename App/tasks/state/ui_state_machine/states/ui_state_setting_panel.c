#include "ui_state_setting_panel.h"

#include "utils.h"

static UiTransitionMapEntry transition_map[] = {
    {.ui_action_id = UI_ACTION_ID_NAVIGATE_RIGHT, .next_ui_state_id = UI_STATE_ID_HOME},
    {.ui_action_id = UI_ACTION_ID_ENTER_ENCODER_A, .next_ui_state_id = UI_STATE_ID_SYSTEM_SETTING},
};

UiState UI_STATE_SETTING_PANEL = {.ui_transition_map = transition_map,
                                  .ui_transition_map_count = ARRAY_COUNT(transition_map),
                                  .ui_state_id = UI_STATE_ID_SETTING};
