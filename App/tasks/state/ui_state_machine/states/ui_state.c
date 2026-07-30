#include "ui_state.h"

UiPanelId UiState_GetUiPanelIdFromUiActionId(UiState* ui_state, UiActionId ui_action_id)
{
    for (size_t i=0; i< ui_state->ui_transition_mappings_count; i++) {
        if (ui_state->ui_transition_mappings[i].ui_action_id == ui_action_id) {
            return ui_state->ui_transition_mappings[i].next_ui_panel_id;
        }
    }
    return UI_PANEL_ID_NONE;
}
