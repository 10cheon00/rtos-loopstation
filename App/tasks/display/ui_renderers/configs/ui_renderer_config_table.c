#include "ui_renderer_config_table.h"

#include "ui_renderer_home_panel.h"
#include "ui_renderer_setting_panel.h"
#include "ui_renderer_system_setting_panel.h"
#include "config_table.h"

#define ENTRIES                                                                                     \
    ConfigTable_1D_ENTRY(UI_PANEL_ID_NONE, NULL),                                                  \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_HOME, Ui_DrawHomePanel),                                  \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SETTING, Ui_DrawSettingPanel),                            \
        ConfigTable_1D_ENTRY(UI_PANEL_ID_SYSTEM_SETTING, Ui_DrawSystemSettingPanel),

ConfigTable_1D_DECLARE_TABLE(UiPanelId, UiPanelRenderFunction, UI_PANEL_ID_COUNT, ENTRIES);
#undef ENTRIES

// ConfigValidator_REGISTER(&ui_panel_render_function_config_map, UiPanelId, UiPanelRenderFunction);

UiPanelRenderFunction UiRendererTable_GetUiPanelRenderFunction(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return ConfigTable_NAME(UiPanelId, UiPanelRenderFunction)[ui_panel_id];
}
