#include "ui_state_parameter_binding.h"

#include "utils.h"

#define UI_PANEL_MAX_PARAMETER_COUNT 4


const ParameterId ui_panel_parameter_binding[UI_PANEL_ID_COUNT][UI_PANEL_MAX_PARAMETER_COUNT] = {
    [UI_PANEL_ID_NONE] = {
        PARAMETER_ID_NONE,
    },
    [UI_PANEL_ID_SETTING] = {
        PARAMETER_ID_SETTING_PANEL_CURSOR,
        PARAMETER_ID_NONE,
        PARAMETER_ID_NONE,
        PARAMETER_ID_NONE
    }
};

const size_t UI_PANEL_PARAMETER_BINDING_COUNT = ARRAY_COUNT(ui_panel_parameter_binding);

ParameterId UiPanelParameterBinding_GetParameterId(UiPanelId ui_panel_id, uint8_t index) {
    if (ui_panel_id < UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return PARAMETER_ID_NONE;
    }
    if (index < PARAMETER_ID_NONE || index >= UI_PANEL_MAX_PARAMETER_COUNT) {
        return PARAMETER_ID_NONE;
    }
    return ui_panel_parameter_binding[ui_panel_id][index];
}
