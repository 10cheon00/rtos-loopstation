#ifndef PANEL_PARAMETER_CONFIG_TABLE_H
#define PANEL_PARAMETER_CONFIG_TABLE_H

#include <stddef.h>

#include "parameter_slot.h"
#include "ui_panel_id.h"
#include "ui_panel_slot_index.h"

typedef struct {
    ParameterSlotConfig slots[UI_PANEL_SLOT_INDEX_COUNT];
} PanelParameterConfig;

ParameterId PanelParameterConfigMap_GetByParameterIndex(UiPanelId ui_panel_id,
                                                        UiPanelSlotIndex index);
const PanelParameterConfig *PanelParameterConfigMap_Get(UiPanelId ui_panel_id);

#endif
