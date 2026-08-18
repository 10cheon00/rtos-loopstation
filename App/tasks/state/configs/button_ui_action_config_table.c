#include "button_ui_action_config_table.h"

#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(BUTTON_ID_NONE, UI_ACTION_ID_NONE),                                       \
        ConfigTable_1D_ENTRY(BUTTON_ID_NULL, UI_ACTION_ID_NULL),                                   \
        ConfigTable_1D_ENTRY(BUTTON_ID_LEFT, UI_ACTION_ID_NAVIGATE_LEFT),                          \
        ConfigTable_1D_ENTRY(BUTTON_ID_RIGHT, UI_ACTION_ID_NAVIGATE_RIGHT),                        \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENTER, UI_ACTION_ID_ENTER),                                 \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_A_PUSH, UI_ACTION_ID_ENTER_ENCODER_A),              \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_B_PUSH, UI_ACTION_ID_ENTER_ENCODER_B),              \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_C_PUSH, UI_ACTION_ID_ENTER_ENCODER_C),              \
        ConfigTable_1D_ENTRY(BUTTON_ID_ENCODER_D_PUSH, UI_ACTION_ID_ENTER_ENCODER_D),              \
        ConfigTable_1D_ENTRY(BUTTON_ID_EXIT, UI_ACTION_ID_EXIT),                                   \
        ConfigTable_1D_ENTRY(BUTTON_ID_IFX_A_TOGGLE, UI_ACTION_ID_TO_IFX),                         \
        ConfigTable_1D_ENTRY(BUTTON_ID_TFX_A_TOGGLE, UI_ACTION_ID_TO_TFX),                         \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_1_EDIT, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_1_PLAY_RECORD, UI_ACTION_ID_NULL),                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_1_STOP, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_2_EDIT, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_2_PLAY_RECORD, UI_ACTION_ID_NULL),                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_2_STOP, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_3_EDIT, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_3_PLAY_RECORD, UI_ACTION_ID_NULL),                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_3_STOP, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_4_EDIT, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_4_PLAY_RECORD, UI_ACTION_ID_NULL),                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_4_STOP, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_5_EDIT, UI_ACTION_ID_NULL),                           \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_5_PLAY_RECORD, UI_ACTION_ID_NULL),                    \
        ConfigTable_1D_ENTRY(BUTTON_ID_TRACK_5_STOP, UI_ACTION_ID_NULL),

ConfigTable_1D_DECLARE_TABLE(ButtonId, UiActionId, BUTTON_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(ButtonId, UiActionId, BUTTON_ID_COUNT, UI_ACTION_ID_NULL,
                                         UI_ACTION_ID_COUNT, CONFIG_TABLE_TYPE_ALLOW_NULL_VALUE);

UiActionId ButtonUiActionConfigMap_Get(ButtonId id)
{
    if (id <= BUTTON_ID_NONE || id >= BUTTON_ID_COUNT) {
        return UI_ACTION_ID_NONE;
    }
    return ConfigTable_1D_GET(ButtonId, UiActionId, id);
}
