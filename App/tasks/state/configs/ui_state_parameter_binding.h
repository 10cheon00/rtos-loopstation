#ifndef UI_STATE_PARAMETER_BINDING_H
#define UI_STATE_PARAMETER_BINDING_H

#include <stddef.h>

#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

ParameterId UiPanelParameterBinding_GetParameterId(UiPanelId ui_panel_id, uint8_t index);

#endif
