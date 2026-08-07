#ifndef UI_RENDERER_UTILS_H
#define UI_RENDERER_UTILS_H

#include "ui_renderer.h"

#define UI_ARROW_FLAG_NONE 0x0
#define UI_ARROW_FLAG_LEFT 0x1
#define UI_ARROW_FLAG_RIGHT 0x2

typedef enum {
    UI_DRAW_PARAMETER_INDEX_NONE = 0,
    UI_DRAW_PARAMETER_INDEX_A,
    UI_DRAW_PARAMETER_INDEX_B,
    UI_DRAW_PARAMETER_INDEX_C,
    UI_DRAW_PARAMETER_INDEX_D,
    UI_DRAW_PARAMETER_INDEX_COUNT,
}UiDrawParameterIndex;

typedef enum {
    UI_PANEL_MENU_ICON_NONE = 0,
    UI_PANEL_MENU_ICON_SYSTEM,
    UI_PANEL_MENU_ICON_DEBUG,
    UI_PANEL_MENU_ICON_COUNT,
} UiPanelMenuIcon;

UiDrawingStatus UI_DrawPanelLayout(u8g2_t* u8g2, const char* panel_name, uint8_t arrow_flag);
UiDrawingStatus UI_DrawParameter(u8g2_t* u8g2, Parameter* parameter, const char* label, UiDrawParameterIndex index);
UiDrawingStatus UI_DrawPanelMenu(u8g2_t* u8g2, UiPanelMenuIcon icon, const char* label, UiDrawParameterIndex index);
#endif
