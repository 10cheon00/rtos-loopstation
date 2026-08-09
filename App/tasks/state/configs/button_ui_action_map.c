#include "button_ui_action_map.h"

#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry button_ui_action_entries[] = {
    {.key = BUTTON_ID_LEFT, .value = UI_ACTION_NAVIGATE_LEFT},
    {.key = BUTTON_ID_RIGHT, .value = UI_ACTION_NAVIGATE_RIGHT},
    {.key = BUTTON_ID_ENTER, .value = UI_ACTION_ENTER},
    {.key = BUTTON_ID_ENCODER_A_PUSH, .value = UI_ACTION_ENTER_ENCODER_A},
    {.key = BUTTON_ID_ENCODER_B_PUSH, .value = UI_ACTION_ENTER_ENCODER_B},
    {.key = BUTTON_ID_ENCODER_C_PUSH, .value = UI_ACTION_ENTER_ENCODER_C},
    {.key = BUTTON_ID_ENCODER_D_PUSH, .value = UI_ACTION_ENTER_ENCODER_D},
    {.key = BUTTON_ID_EXIT, .value = UI_ACTION_EXIT},
    {.key = BUTTON_ID_IFX_A_TOGGLE, .value = UI_ACTION_TO_IFX},
    {.key = BUTTON_ID_TFX_A_TOGGLE, .value = UI_ACTION_TO_TFX}};

static ConfigMap button_ui_action_map = {
    .count = ARRAY_COUNT(button_ui_action_entries),
    .entries = button_ui_action_entries,
};

ConfigValidator_REGISTER(&button_ui_action_map, ButtnId, UiActionId);

UiActionId ButtonUiActionMap_Get(UiActionId id)
{
    UiActionId ui_action_id;
    if (ConfigMap_Get(&button_ui_action_map, id, (Value_t *)&ui_action_id) ==
        CONFIG_MAP_RESULT_OK) {
        return ui_action_id;
    }
    return UI_ACTION_NONE;
}
