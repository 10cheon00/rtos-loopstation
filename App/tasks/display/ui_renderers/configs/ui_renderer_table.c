#include "ui_renderer.h"

#include "ui_renderer_home_panel.h"
#include "ui_renderer_setting_panel.h"
#include "ui_renderer_system_setting_panel.h"

static UiPanelRenderFunction ui_panel_render_function_table[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE]=0,
    [UI_PANEL_ID_HOME]=Ui_DrawHomePanel,
    [UI_PANEL_ID_SETTING]=Ui_DrawSettingPanel,
    [UI_PANEL_ID_SYSTEM_SETTING]=Ui_DrawSystemSettingPanel,
};

UiPanelRenderFunction UiRendererTable_GetUiPanelRenderFunction(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return ui_panel_render_function_table[UI_PANEL_ID_NONE];
    }
    return ui_panel_render_function_table[ui_panel_id];
}
