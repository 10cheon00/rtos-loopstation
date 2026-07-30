#ifndef UI_PANEL_ID_PARAMETER_BINDING_H
#define UI_PANEL_ID_PARAMETER_BINDING_H

#define UI_PANEL_MAX_PARAMETER_COUNT 4

#include <stddef.h>

#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

ParameterId UiPanelIdParameterIdBinding_GetParameterId(UiPanelId ui_panel_id, uint8_t index);
ParameterId *UiPanelIdParameterIdBinding_GetParameterIdsFromUiPanelId(UiPanelId ui_panel_id);

#endif
