#ifndef PANEL_PARAMETER_TABLE_H
#define PANEL_PARAMETER_TABLE_H

#define UI_PANEL_MAX_PARAMETER_COUNT 4

#include <stddef.h>

#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

ParameterId PanelParameterTable_GetParameterId(UiPanelId ui_panel_id, uint8_t index);
ParameterId *PanelParameterTable_GetParameterIds(UiPanelId ui_panel_id);

uint8_t PanelParameterTable_IsParameterIdInTableFromUiPanelId(UiPanelId ui_panel_id,
                                                              ParameterId parameter_id);
#endif
