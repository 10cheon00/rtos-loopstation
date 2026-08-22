#include "ui_panel_ui_state_config_table.h"

#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(UI_STATE_ID, (Value_t)NULL),                                         \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_HOME, (Value_t) & UI_STATE_HOME_PANEL),                   \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SETTING, (Value_t) & UI_STATE_SETTING_PANEL),             \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SYSTEM_SETTING,                                           \
                             (Value_t) & UI_STATE_SYSTEM_SETTING_PANEL),

ConfigTable_1D_DECLARE_TABLE(UiStateId, UiStatePointer, UI_PANEL_ID_COUNT, ENTRIES);

ConfigValidator_REGISTER_CONFIG_TABLE_1D(UiStateId, UiStatePointer, UI_PANEL_ID_COUNT, 0,
                                         UINT32_MAX, CONFIG_TABLE_TYPE_NO_NULL_VALUE);

UiStatePointer UiPanelUiStateConfigMap_Get(UiStateId id)
{
    if (id <= UI_STATE_ID || id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return (UiStatePointer)ConfigTable_1D_GET(UiStateId, UiStatePointer, id);
}
