#include "ui_panel_ui_state_config_table.h"

#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

#include "config_table.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(UI_PANEL_ID_NONE, 0),                                                     \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_HOME, &UI_STATE_HOME_PANEL),                              \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SETTING, &UI_STATE_SETTING_PANEL),                        \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SYSTEM_SETTING, &UI_STATE_SYSTEM_SETTING_PANEL),

ConfigTable_1D_DECLARE_TABLE(UiPanelId, UiStatePointer, UI_PANEL_ID_COUNT, ENTRIES);

// ConfigValidator_REGISTER(&ui_panel_ui_state_config_map, UiPanelId, UiStatePointer);

UiStatePointer UiPanelUiStateConfigMap_Get(UiPanelId id)
{
    if (id <= UI_PANEL_ID_NONE || id >= UI_PANEL_ID_COUNT)
    {
        return NULL;
    }
    return ConfigTable_1D_GET(UiPanelId, UiStatePointer, id);
}
