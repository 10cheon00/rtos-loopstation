#include "ui_renderer_setting_panel.h"

#include "knob_widget.h"
#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawSystemSettingPanel(u8g2_t *u8g2, Parameter *parameters)
{
    // 여기서는 인자로 전달된 시스템 밝기를 수정함
    uint8_t value = parameters[0].current;
    u8g2_SetContrast(u8g2, value);

    u8g2_ClearBuffer(u8g2);
    UI_DrawPanelLayout(u8g2, "SYSTEM SETTING", UI_ARROW_FLAG_NONE);
    
    UI_DrawParameter(u8g2, &parameters[0], "LCD CO-\nNSTRAST", UI_PANEL_SLOT_INDEX_A);
    // u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
    // UiWidget_DrawKnobWidget(u8g2, 8, 40, &parameters[0]);
    // u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    // u8g2_DrawStr(u8g2, 2, 46, "LCD CO-");
    // u8g2_DrawStr(u8g2, 5, 52, "NTRAST");
    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
