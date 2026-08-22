#ifndef UI_RENDERER_UTILS_H
#define UI_RENDERER_UTILS_H

#include "ui_renderer.h"
#include "ui_state_slot_index.h"

#define UI_ARROW_FLAG_NONE 0x0
#define UI_ARROW_FLAG_LEFT 0x1
#define UI_ARROW_FLAG_RIGHT 0x2

typedef enum {
    UI_PANEL_SLOT_ICON_ID_NONE = 0,
    UI_PANEL_SLOT_ICON_ID_SYSTEM,
    UI_PANEL_SLOT_ICON_ID_DEBUG,
    UI_PANEL_SLOT_ICON_ID_COUNT,
} UiPanelSlotIconId;

UiDrawingStatus UI_DrawPanelLayout(u8g2_t *u8g2, const char *panel_name, uint8_t arrow_flag);
UiDrawingStatus UI_DrawParameter(u8g2_t *u8g2, Parameter *parameter, const char *label,
                                 UiStateSlotIndex index);
UiDrawingStatus UI_DrawPanelMenu(u8g2_t *u8g2, UiPanelSlotIconId icon, const char *label,
                                 UiStateSlotIndex index);
#endif
