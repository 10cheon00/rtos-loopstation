#include "ui_panel_renderer.h"

#include "ui_home_panel.h"
#include "ui_setting_panel.h"

static UiPanelRenderFunction ui_panel_render_function_table[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE]=0,
    [UI_PANEL_ID_HOME]=Ui_DrawHomePanel,
    [UI_PANEL_ID_SETTING]=Ui_DrawSettingPanel,
};

UiPanelRenderFunction UiPanelRendererTable_GetUiPanelRenderFunction(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return ui_panel_render_function_table[UI_PANEL_ID_NONE];
    }
    return ui_panel_render_function_table[ui_panel_id];
}
