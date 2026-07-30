#include "control_button_id_ui_state_event_id_mapping.h"

#include "utils.h"

ControlButtonIdUiStateEventIdMapping control_button_id_ui_state_event_id_mapping[] = {
    {
        .control_button_id = CONTROL_BUTTON_ID_LEFT,
        .ui_state_event_id = UI_STATE_EVENT_NAVIGATE_LEFT
    },
    {
        .control_button_id = CONTROL_BUTTON_ID_RIGHT,
        .ui_state_event_id = UI_STATE_EVENT_NAVIGATE_RIGHT
    },
    {
        .control_button_id = CONTROL_BUTTON_ID_ENTER,
        .ui_state_event_id = UI_STATE_EVENT_ENTER
    },
    {
        .control_button_id = CONTROL_BUTTON_ID_EXIT,
        .ui_state_event_id = UI_STATE_EVENT_EXIT
    },
    {
        .control_button_id = CONTROL_BUTTON_ID_IFX_A_TOGGLE,
        .ui_state_event_id = UI_STATE_EVENT_TO_IFX
    },
    {
        .control_button_id = CONTROL_BUTTON_ID_TFX_A_TOGGLE,
        .ui_state_event_id = UI_STATE_EVENT_TO_TFX
    }
};

size_t control_button_id_ui_state_event_id_mapping_count = ARRAY_COUNT(control_button_id_ui_state_event_id_mapping);
