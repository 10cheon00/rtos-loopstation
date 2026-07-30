#ifndef CONTROL_BUTTON_ID_UI_ACTION_ID_MAPPING_H
#define CONTROL_BUTTON_ID_UI_ACTION_ID_MAPPING_H

#include <stddef.h>

#include "control_button_id.h"
#include "ui_state.h"

typedef struct {
    ControlButtonId control_button_id;
    UiActionId ui_action_id;
} ControlButtonIdUiActionIdMapping;

extern ControlButtonIdUiActionIdMapping control_button_id_ui_action_id_mapping[];
extern size_t control_button_id_ui_action_id_mapping_count;

#endif
