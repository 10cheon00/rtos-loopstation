#include "button_ui_action_map.h"

#include "utils.h"

ButtonUiActionMapEntry button_ui_action_map[] = {
    {.button_id = BUTTON_ID_LEFT, .ui_action_id = UI_ACTION_NAVIGATE_LEFT},
    {.button_id = BUTTON_ID_RIGHT, .ui_action_id = UI_ACTION_NAVIGATE_RIGHT},
    {.button_id = BUTTON_ID_ENTER, .ui_action_id = UI_ACTION_ENTER},
    {.button_id = BUTTON_ID_EXIT, .ui_action_id = UI_ACTION_EXIT},
    {.button_id = BUTTON_ID_IFX_A_TOGGLE, .ui_action_id = UI_ACTION_TO_IFX},
    {.button_id = BUTTON_ID_TFX_A_TOGGLE, .ui_action_id = UI_ACTION_TO_TFX}};

size_t button_ui_action_map_count = ARRAY_COUNT(button_ui_action_map);
