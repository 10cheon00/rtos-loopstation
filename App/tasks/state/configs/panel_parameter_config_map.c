#include "panel_parameter_config_map.h"

#include "config_map.h"
#include "config_validator.h"

static ParameterId ui_panel_none_params[UI_PANEL_MAX_PARAMETER_COUNT] = {
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
};
static ParameterId ui_panel_home_params[UI_PANEL_MAX_PARAMETER_COUNT] = {
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
};
static ParameterId ui_panel_setting_params[UI_PANEL_MAX_PARAMETER_COUNT] = {
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
};
static ParameterId ui_panel_system_setting_params[UI_PANEL_MAX_PARAMETER_COUNT] = {
    PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
    PARAMETER_ID_IFX_A_STATE,
    PARAMETER_ID_NONE,
    PARAMETER_ID_NONE,
};

static ConfigMapEntry panel_parameter_config_map_entries[] = {
    {.key = UI_PANEL_ID_NONE, .value = (uint32_t)ui_panel_none_params},
    {.key = UI_PANEL_ID_HOME, .value = (uint32_t)ui_panel_home_params},
    {.key = UI_PANEL_ID_SETTING, .value = (uint32_t)ui_panel_setting_params},
    {.key = UI_PANEL_ID_SYSTEM_SETTING, .value = (uint32_t)ui_panel_system_setting_params}};

static ConfigMap panel_parameter_config_map = {.entries = panel_parameter_config_map_entries,
                                               .count =
                                                   ARRAY_COUNT(panel_parameter_config_map_entries)};

ConfigValidator_REGISTER(&panel_parameter_config_map, ParameterId, UiPanelParameters);

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id, uint8_t index)
{
    Value_t value;
    if (ConfigMap_Get(&panel_parameter_config_map, ui_panel_id, &value) != CONFIG_MAP_RESULT_OK) {
        return PARAMETER_ID_NONE;
    }
    return (ParameterId)((UiPanelParameters)value)[index];
}

UiPanelParameters PanelParameterConfigMap_Get(UiPanelId ui_panel_id)
{
    Value_t value;
    if (ConfigMap_Get(&panel_parameter_config_map, ui_panel_id, &value) != CONFIG_MAP_RESULT_OK) {
        return PARAMETER_ID_NONE;
    }
    return (UiPanelParameters)value;
}
