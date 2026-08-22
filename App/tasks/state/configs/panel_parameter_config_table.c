#include "panel_parameter_config_table.h"

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
    return config->slots[index].parameter_id;
}

const PanelParameterConfig *PanelParameterConfigMap_Get(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return &panel_parameter_config_table[ui_panel_id];
}
