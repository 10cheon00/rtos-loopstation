#include "button_ui_action_config_table.h"

#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(BUTTON_ID_LEFT, UI_ACTION_NAVIGATE_LEFT),                                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_RIGHT, UI_ACTION_NAVIGATE_RIGHT),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENTER, UI_ACTION_ENTER),                                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_A_PUSH, UI_ACTION_ENTER_ENCODER_A),                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_B_PUSH, UI_ACTION_ENTER_ENCODER_B),                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_C_PUSH, UI_ACTION_ENTER_ENCODER_C),                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_D_PUSH, UI_ACTION_ENTER_ENCODER_D),                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_EXIT, UI_ACTION_EXIT),                                      \
        ConfigTable_1D_ENTRY(BUTTON_ID_IFX_A_TOGGLE, UI_ACTION_TO_IFX),                            \
        ConfigTable_1D_ENTRY(BUTTON_ID_TFX_A_TOGGLE, UI_ACTION_TO_TFX),

ConfigTable_1D_DECLARE_TABLE(ButtonId, UiActionId, BUTTON_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(ButtonId, UiActionId, BUTTON_ID_COUNT, UI_ACTION_NONE,
                                         UI_ACTION_COUNT, CONFIG_TABLE_TYPE_NO_NULL_VALUE);

UiActionId ButtonUiActionConfigMap_Get(ButtonId id)
{
    if (id <= BUTTON_ID_NONE || id >= BUTTON_ID_COUNT) {
        return UI_ACTION_NONE;
    }
    return ConfigTable_1D_GET(ButtonId, UiActionId, id);
}
