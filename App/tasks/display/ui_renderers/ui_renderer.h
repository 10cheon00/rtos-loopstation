#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "gmg12864_lcd.h"
#include "parameter.h"
#include "parameter_id.h"
#include "ui_panel_id.h"

extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

typedef enum {
    UI_DRAWING_STATUS_OK = 0,
    UI_DRAWING_STATUS_ERROR,
} UiDrawingStatus;

typedef UiDrawingStatus (*UiPanelRenderFunction)(u8g2_t *, Parameter *parameter_a,
                                                 Parameter *parameter_b,
                                                 Parameter *parameter_c,
                                                 Parameter *parameter_d);

#endif
