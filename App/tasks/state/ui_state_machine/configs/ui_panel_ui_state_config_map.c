#include "ui_panel_ui_state_config_map.h"

#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry ui_panel_ui_state_config_map_entries[] = {
    {.key = UI_PANEL_ID_NONE, .value = (Value_t)0},
    {.key = UI_PANEL_ID_HOME, .value = (Value_t)&UI_STATE_HOME_PANEL},
    {.key = UI_PANEL_ID_SETTING, .value = (Value_t)&UI_STATE_SETTING_PANEL},
    {.key = UI_PANEL_ID_SYSTEM_SETTING, .value = (Value_t)&UI_STATE_SYSTEM_SETTING_PANEL},
};

static ConfigMap ui_panel_ui_state_config_map = {
    .entries = ui_panel_ui_state_config_map_entries,
    .count = ARRAY_COUNT(ui_panel_ui_state_config_map_entries)};

ConfigValidator_REGISTER(&ui_panel_ui_state_config_map, UiPanelId, UiStatePointer);

UiState *UiPanelUiStateConfigMap_Get(UiPanelId ui_panel_id)
{
    Value_t value;
    if (ConfigMap_Get(&ui_panel_ui_state_config_map, ui_panel_id, &value) != CONFIG_MAP_RESULT_OK) {
        return NULL;
    }
    return (UiState *)value;
}
