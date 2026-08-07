#include "ui_renderer_setting_panel.h"

#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawSettingPanel(u8g2_t *u8g2, Parameter *parameters)
{
    u8g2_ClearBuffer(u8g2);

    UI_DrawPanelLayout(u8g2, "SYSTEM", UI_ARROW_FLAG_RIGHT);

    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_PANEL_SLOT_INDEX_A);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_DEBUG, "DEBUG\nHW", UI_PANEL_SLOT_INDEX_B);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_PANEL_SLOT_INDEX_C);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_PANEL_SLOT_INDEX_D);
    // u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
    // u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    // u8g2_DrawGlyph(u8g2, 8, 40, 104);
    // u8g2_DrawGlyph(u8g2, 40, 40, 282);
    // u8g2_DrawGlyph(u8g2, 72, 40, 87);
    // u8g2_DrawGlyph(u8g2, 104, 40, 87);
    // u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    // u8g2_DrawStr(u8g2, 2, 46, "SYSTEM");
    // u8g2_DrawStr(u8g2, 36, 46, "DEBUG");
    // u8g2_DrawStr(u8g2, 28, 52, "HARDWARE");
    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
