#include "panel_parameter_config_table.h"

/**
 * 여기에 정의된 파라미터들은 패널에서 보여줄 파라미터가 고정되어 있을 때에만 사용자에게 보여진다.
 * 근데 생각해보면 모든 파라미터들은 고정되어 있지 않다고 생각되는데,,,
 */
static const PanelParameterConfig panel_parameter_config_table[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE] = {.slots = {{PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL}}},
    [UI_PANEL_ID_HOME] = {.slots = {{PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL},
                                    {PARAMETER_ID_NULL, NULL}}},
    [UI_PANEL_ID_SETTING] = {.slots = {{PARAMETER_ID_NULL, NULL},
                                       {PARAMETER_ID_NULL, NULL},
                                       {PARAMETER_ID_NULL, NULL},
                                       {PARAMETER_ID_NULL, NULL}}},
    [UI_PANEL_ID_SYSTEM_SETTING] = {.slots = {{PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
                                               "LCD CO-\nNTRAST"},
                                              {PARAMETER_ID_IFX_A_STATE, "IFX A"},
                                              {PARAMETER_ID_NULL, NULL},
                                              {PARAMETER_ID_NULL, NULL}}},
};

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id,
                                                        UiPanelSlotIndex index)
{
    const PanelParameterConfig *config = PanelParameterConfigMap_Get(ui_panel_id);
    if (config == NULL || index >= UI_PANEL_SLOT_INDEX_COUNT) {
        return PARAMETER_ID_NULL;
    }
    return config->slots[index].id;
}

const PanelParameterConfig *PanelParameterConfigMap_Get(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return &panel_parameter_config_table[ui_panel_id];
}
