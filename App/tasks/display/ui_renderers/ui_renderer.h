#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "gmg12864_lcd.hpp"
#include "display_messages.h"
#include "parameter.h"
#include "parameter_id.h"
#include "ui_state_id.h"

extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

typedef enum {
    UI_DRAWING_STATUS_OK = 0,
    UI_DRAWING_STATUS_ERROR,
} UiDrawingStatus;

typedef UiDrawingStatus (*UiPanelRenderFunction)(u8g2_t *, ParameterRenderPayload *);

UiDrawingStatus UI_DrawPanelLayout(u8g2_t *u8g2, const char *panel_name, PageNavigationFlag flag);
UiDrawingStatus UI_DrawParameter(u8g2_t *u8g2, Parameter *parameter, const char *label,
                                 UiStateSlotIndex index);
UiDrawingStatus UI_DrawMenu(u8g2_t *u8g2, MenuIconId icon_id, const char *label,
                                 UiStateSlotIndex index);
#endif
