#ifndef UI_STATE_PARAMETER_BINDING_H
#define UI_STATE_PARAMETER_BINDING_H

#include <stddef.h>

#include "parameter.h"
#include "ui_panel_id.h"
#include "parameter_id.h"

#define UI_PANEL_MAX_PARAMETER_COUNT 4

typedef struct {
    UiPanelId ui_panel_id;
    ParameterId parameter_ids[UI_PANEL_MAX_PARAMETER_COUNT]; // 한 패널에서 최대 4개의 파라미터를 설정 가능함
} UiPanelParameterBinding;

extern const UiPanelParameterBinding ui_panel_parameter_binding[];
extern const size_t UI_PANEL_PARAMETER_BINDING_COUNT;

#endif
