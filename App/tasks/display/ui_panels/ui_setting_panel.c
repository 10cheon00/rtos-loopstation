#include "ui_setting_panel.h"

UiDrawingStatus Ui_DrawSettingPanel(u8g2_t *u8g2, Parameter *parameters)
{
    char str[6];
    
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_DrawStr(u8g2, 1, 6, "SETTING PANEL");
    u8g2_DrawLine(u8g2, 0, 7, 128, 7);
    UI_DrawArrowRight4x5(u8g2, 122, 6);
    u8g2_SetFont(u8g2, u8g2_font_tiny5_tr);
    UI_ConvertNumberToString(parameters[0].current, str, 6);
    u8g2_DrawStr(u8g2, 10, 15, str);

    u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
    u8g2_DrawGlyph(u8g2, 8, 40, 104);
    u8g2_DrawGlyph(u8g2, 40, 40, 282);
    u8g2_DrawGlyph(u8g2, 72, 40, 87);
    u8g2_DrawGlyph(u8g2, 104, 40, 87);
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_DrawStr(u8g2, 2, 46, "SYSTEM");
    u8g2_DrawStr(u8g2, 36, 46, "DEBUG");
    u8g2_DrawStr(u8g2, 28, 52, "HARDWARE");
    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
