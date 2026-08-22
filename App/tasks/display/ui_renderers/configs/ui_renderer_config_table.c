#include "ui_renderer_config_table.h"

#include "ui_renderer_home_panel.h"
#include "ui_renderer_setting_panel.h"
#include "ui_renderer_system_setting_panel.h"
#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(UI_STATE_ID_NONE, (Value_t)NULL),                                        \
        ConfigTable_1D_ENTRY(UI_STATE_ID_HOME, (Value_t)Ui_DrawHomePanel),                         \
        ConfigTable_1D_ENTRY(UI_STATE_ID_SETTING, (Value_t)Ui_DrawSettingPanel),                   \
        ConfigTable_1D_ENTRY(UI_STATE_ID_SYSTEM_SETTING, (Value_t)Ui_DrawSystemSettingPanel),

ConfigTable_1D_DECLARE_TABLE(UiStateId, UiPanelRenderFunction, UI_STATE_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(UiStateId, UiPanelRenderFunction, UI_STATE_ID_COUNT, 0,
                                         UINT32_MAX, CONFIG_TABLE_TYPE_NO_NULL_VALUE);

UiPanelRenderFunction UiRendererTable_GetUiPanelRenderFunction(UiStateId ui_state_id)
{
    if (ui_state_id <= UI_STATE_ID_NONE || ui_state_id >= UI_STATE_ID_COUNT) {
        return NULL;
    }
    return (UiPanelRenderFunction)ConfigTable_1D_GET(UiStateId, UiPanelRenderFunction, ui_state_id);
}
