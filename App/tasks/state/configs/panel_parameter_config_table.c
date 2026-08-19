#include "panel_parameter_config_table.h"

#include "config_table.h"

#define ENTRIES                                                                                    \
    ConfigTable_2D_ENTRY(UI_PANEL_ID_NONE, PARAMETER_ID_NULL, PARAMETER_ID_NULL,                   \
                         PARAMETER_ID_NULL, PARAMETER_ID_NULL),                                    \
        ConfigTable_2D_ENTRY(UI_PANEL_ID_HOME, PARAMETER_ID_NULL, PARAMETER_ID_NULL,               \
                             PARAMETER_ID_NULL, PARAMETER_ID_NULL),                                \
        ConfigTable_2D_ENTRY(UI_PANEL_ID_SYSTEM_SETTING,                                           \
                             PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST, PARAMETER_ID_IFX_A_STATE,  \
                             PARAMETER_ID_NULL, PARAMETER_ID_NULL),                                \
        ConfigTable_2D_ENTRY(UI_PANEL_ID_SETTING, PARAMETER_ID_NULL, PARAMETER_ID_NULL,            \
                             PARAMETER_ID_NULL, PARAMETER_ID_NULL),

ConfigTable_2D_DECLARE_TABLE(UiPanelId, ParameterId, UI_PANEL_ID_COUNT,
                             UI_PANEL_MAX_PARAMETER_COUNT, ENTRIES);
#undef ENTRIES

// ConfigValidator_REGISTER(&panel_parameter_config_map, ParameterId, UiPanelParameters);

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id, uint8_t index)
{
    UiPanelParameters parameters = PanelParameterConfigMap_Get(ui_panel_id);
    if (parameters == NULL || index < 0 || index >= UI_PANEL_MAX_PARAMETER_COUNT) {
        // TODO:
        // 엔코더 인덱스가 [0,4)로 되어 있어서 조건을 0미만으로 수정했음..엔코더 인덱스 역시 enum으로 관리하기
        return PARAMETER_ID_NULL;
    }
    return parameters[index];
}

UiPanelParameters PanelParameterConfigMap_Get(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return (UiPanelParameters)ConfigTable_2D_GET_ROW(UiPanelId, ParameterId, ui_panel_id);
}
