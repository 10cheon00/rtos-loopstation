#ifndef UI_PANEL_RENDERER_H
#define UI_PANEL_RENDERER_H

#include "ui.h"
#include "parameter.h"
#include "ui_panel_id.h"

typedef UiDrawingStatus(*Ui_UiPanelRenderFunction)(u8g2_t*,Parameter*);

extern const Ui_UiPanelRenderFunction ui_panel_render_mappings[UI_PANEL_ID_COUNT];

#endif
