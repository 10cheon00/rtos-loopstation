#include "ui_home_panel.h"

UI_DRAWING_STATUS Ui_DrawHomePanel(u8g2_t *u8g2, Parameter *parameters)
{
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_ClearBuffer(u8g2);
    u8g2_DrawStr(u8g2, 1, 6, "HOME PANEL");
    u8g2_DrawLine(u8g2, 0, 7, 128, 7);
    UI_DrawArrowLeft4x5(u8g2, 117, 6);
    UI_DrawArrowRight4x5(u8g2, 122, 6);

    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
