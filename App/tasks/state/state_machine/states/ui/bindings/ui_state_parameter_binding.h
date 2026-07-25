#ifndef UI_STATE_PARAMETER_BINDING_H
#define UI_STATE_PARAMETER_BINDING_H

#include <stddef.h>

#include "parameter.h"
#include "ui_panel_id.h"
#include "parameter_id.h"

typedef struct {
    UiPanelId ui_panel_id;
    ParameterId parameter_id;
} UiPanelParameterBinding;

extern const UiPanelParameterBinding ui_panel_parameter_binding[];
extern const size_t UI_PANEL_PARAMETER_BINDING_COUNT;

#endif
