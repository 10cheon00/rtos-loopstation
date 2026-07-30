#ifndef BUTTON_UI_ACTION_MAP_H
#define BUTTON_UI_ACTION_MAP_H

#include <stddef.h>

#include "button_id.h"
#include "ui_state.h"

typedef struct {
    ButtonId button_id;
    UiActionId ui_action_id;
} ButtonUiActionMapEntry;

extern ButtonUiActionMapEntry button_ui_action_map[];
extern size_t button_ui_action_map_count;

#endif
