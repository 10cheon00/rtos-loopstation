#include "ui_renderer_config_map.h"

#include "ui_renderer_home_panel.h"
#include "ui_renderer_setting_panel.h"
#include "ui_renderer_system_setting_panel.h"
#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry ui_panel_render_function_config_map_entries[] = {
    {.key = UI_PANEL_ID_NONE, .value = 0},
    {.key = UI_PANEL_ID_HOME, .value = (Value_t)Ui_DrawHomePanel},
    {.key = UI_PANEL_ID_SETTING, .value = (Value_t)Ui_DrawSettingPanel},
    {.key = UI_PANEL_ID_SYSTEM_SETTING, .value = (Value_t)Ui_DrawSystemSettingPanel},
};

static ConfigMap ui_panel_render_function_config_map = {
    .entries = ui_panel_render_function_config_map_entries,
    .count = ARRAY_COUNT(ui_panel_render_function_config_map_entries)};

ConfigValidator_REGISTER(&ui_panel_render_function_config_map, UiPanelId, UiPanelRenderFunction);

UiPanelRenderFunction UiRendererTable_GetUiPanelRenderFunction(UiPanelId ui_panel_id)
{
    Value_t value;
    if (ConfigMap_Get(&ui_panel_render_function_config_map, ui_panel_id, &value) !=
        CONFIG_MAP_RESULT_OK) {
        return NULL;
    }
    return (UiPanelRenderFunction)value;
}
