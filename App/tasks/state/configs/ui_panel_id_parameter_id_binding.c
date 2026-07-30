#include "ui_panel_id_parameter_id_binding.h"

#include "utils.h"

const ParameterId bindings[UI_PANEL_ID_COUNT][UI_PANEL_MAX_PARAMETER_COUNT] = {
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

const size_t bindings_COUNT = ARRAY_COUNT(bindings);

ParameterId UiPanelIdParameterIdBinding_GetParameterId(UiPanelId ui_panel_id, uint8_t index) {
    if (ui_panel_id < UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return PARAMETER_ID_NONE;
    }
    if (index < PARAMETER_ID_NONE || index >= UI_PANEL_MAX_PARAMETER_COUNT) {
        return PARAMETER_ID_NONE;
    }
    return bindings[ui_panel_id][index];
}

ParameterId* UiPanelIdParameterIdBinding_GetParameterIdsFromUiPanelId(UiPanelId ui_panel_id) {
    if (ui_panel_id < UI_PANEL_ID_NONE || ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return bindings[ui_panel_id];
}