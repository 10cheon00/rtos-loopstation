#include "ui_panel_renderer.h"

#include "ui_home_panel.h"
#include "ui_setting_panel.h"

const UiPanelRenderFunction ui_panel_render_function_table[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE]=0,
    [UI_PANEL_ID_HOME]=Ui_DrawHomePanel,
    [UI_PANEL_ID_SETTING]=Ui_DrawSettingPanel,
};