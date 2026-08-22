#ifndef PANEL_PARAMETER_CONFIG_TABLE_H
#define PANEL_PARAMETER_CONFIG_TABLE_H

#include <stddef.h>

#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"
#include "ui_panel_slot_index.h"

typedef struct {
    ParameterId parameter_id;
    const char *label;
} PanelParameterSlotConfig;

typedef struct {
    PanelParameterSlotConfig slots[UI_PANEL_SLOT_INDEX_COUNT];
} PanelParameterConfig;

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id,
                                                        UiPanelSlotIndex index);
const PanelParameterConfig *PanelParameterConfigMap_Get(UiPanelId ui_panel_id);

#endif
