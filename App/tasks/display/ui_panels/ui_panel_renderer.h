#ifndef UI_PANEL_RENDERER_H
#define UI_PANEL_RENDERER_H

#include "ui.h"
#include "parameter.h"
#include "ui_panel_id.h"

typedef UiDrawingStatus(*UiPanelRenderFunction)(u8g2_t*,Parameter*);

extern const UiPanelRenderFunction ui_panel_render_function_table[UI_PANEL_ID_COUNT];

#endif
