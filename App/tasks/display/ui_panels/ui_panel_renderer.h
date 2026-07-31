#ifndef UI_PANEL_RENDERER_H
#define UI_PANEL_RENDERER_H

#include "ui.h"
#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

typedef UiDrawingStatus (*UiPanelRenderFunction)(u8g2_t *, Parameter *);

UiPanelRenderFunction UiPanelRendererTable_GetUiPanelRenderFunction(UiPanelId ui_panel_id);

#endif
