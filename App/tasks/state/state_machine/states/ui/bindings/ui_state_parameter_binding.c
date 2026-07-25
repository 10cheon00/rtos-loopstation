#include "ui_state_parameter_binding.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

const UiPanelParameterBinding ui_panel_parameter_binding[] = {
    {
        .ui_panel_id = UI_PANEL_ID_SETTING,
        .parameter_id = PARAMETER_ID_SETTING_PANEL_CURSOR,
    },
};

const size_t UI_PANEL_PARAMETER_BINDING_COUNT = ARRAY_COUNT(ui_panel_parameter_binding);
