#ifndef PANEL_PARAMETER_CONFIG_TABLE_H
#define PANEL_PARAMETER_CONFIG_TABLE_H

#include <stddef.h>

#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

#define UI_PANEL_MAX_PARAMETER_COUNT 4
typedef ParameterId *UiPanelParameters;

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id, uint8_t index);
UiPanelParameters PanelParameterConfigMap_Get(UiPanelId ui_panel_id);

#endif
