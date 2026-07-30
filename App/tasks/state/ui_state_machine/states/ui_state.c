#include "ui_state.h"

UiPanelId UiState_GetUiPanelIdFromUiStateEventId(UiState* ui_state, UiStateEventId ui_state_event_id)
{
    for (size_t i=0; i< ui_state->ui_transition_mappings_count; i++) {
        if (ui_state->ui_transition_mappings[i].ui_state_event_id == ui_state_event_id) {
            return ui_state->ui_transition_mappings[i].next_ui_panel_id;
        }
    }
    return UI_PANEL_ID_NONE;
}
