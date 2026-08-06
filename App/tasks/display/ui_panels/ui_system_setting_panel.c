#include "ui_setting_panel.h"

#include "knob_widget.h"

UiDrawingStatus Ui_DrawSystemSettingPanel(u8g2_t *u8g2, Parameter *parameters)
{
    // 여기서는 인자로 전달된 시스템 밝기를 수정함
    uint8_t value = parameters[0].current;
    u8g2_SetContrast(u8g2, value);

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_DrawStr(u8g2, 1, 6, "SYSTEM SETTING");
    u8g2_DrawLine(u8g2, 0, 7, 128, 7);
    UI_DrawArrowRight4x5(u8g2, 122, 6);
    u8g2_SetFont(u8g2, u8g2_font_tiny5_tr);

    u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
    UiWidget_DrawKnobWidget(u8g2, 8, 40, &parameters[0]);
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_DrawStr(u8g2, 2, 46, "LCD CO-");
    u8g2_DrawStr(u8g2, 5, 52, "NTRAST");
    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
