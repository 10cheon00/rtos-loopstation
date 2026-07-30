#ifndef CONTROL_BUTTON_ID_UI_STATE_EVENT_ID_MAPPING_H
#define CONTROL_BUTTON_ID_UI_STATE_EVENT_ID_MAPPING_H

#include <stddef.h>

#include "control_button_id.h"
#include "ui_state.h"

typedef struct {
    ControlButtonId control_button_id;
    UiStateEventId ui_state_event_id;
} ControlButtonIdUiStateEventIdMapping;

extern ControlButtonIdUiStateEventIdMapping control_button_id_ui_state_event_id_mapping[];
extern size_t control_button_id_ui_state_event_id_mapping_count;

#endif
