#include "panel_parameter_table.h"

#include "utils.h"

const ParameterId table[UI_PANEL_ID_COUNT][UI_PANEL_MAX_PARAMETER_COUNT] = {
    [UI_PANEL_ID_NONE] =
        {
            PARAMETER_ID_NONE,
        },
    [UI_PANEL_ID_SETTING] = {PARAMETER_ID_SETTING_PANEL_CURSOR, PARAMETER_ID_NONE,
                             PARAMETER_ID_NONE, PARAMETER_ID_NONE},
    [UI_PANEL_ID_SYSTEM_SETTING] = {PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST, PARAMETER_ID_NONE,
                                    PARAMETER_ID_NONE, PARAMETER_ID_NONE},
};

const size_t TABLE_COUNT = ARRAY_COUNT(table);

ParameterId PanelParameterTable_GetParameterId(UiPanelId ui_panel_id, uint8_t index)
{
    if (ui_panel_id < UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return PARAMETER_ID_NONE;
    }
    if (index < PARAMETER_ID_NONE || index >= UI_PANEL_MAX_PARAMETER_COUNT) {
        return PARAMETER_ID_NONE;
    }
    return table[ui_panel_id][index];
}

ParameterId *PanelParameterTable_GetParameterIds(UiPanelId ui_panel_id)
{
    if (ui_panel_id < UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return table[ui_panel_id];
}

// TODO:
// 반환값을 1,0으로 하지 않고, 모든 코드들이 공유하는 반환값(HAL_StatusTypeDef같은거)을 정의하기
uint8_t PanelParameterTable_IsParameterIdInTableFromUiPanelId(UiPanelId ui_panel_id,
                                                              ParameterId parameter_id)
{
    for (uint8_t i = 0; i < UI_PANEL_MAX_PARAMETER_COUNT; i++) {
        if (table[ui_panel_id][i] == parameter_id) {
            return 1;
        }
    }
    return 0;
}
