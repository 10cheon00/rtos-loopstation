#include "ui_renderer_system_setting_panel.h"

#include "knob_widget.h"
#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawSystemSettingPanel(u8g2_t *u8g2, Parameter *parameter_a,
                                          Parameter *parameter_b, Parameter *parameter_c,
                                          Parameter *parameter_d)
{
    // 여기서는 인자로 전달된 시스템 밝기를 수정함
    uint8_t value = parameter_a->current;
    u8g2_SetContrast(u8g2, value);

    UI_DrawPanelLayout(u8g2, "SYSTEM SETTING", UI_ARROW_FLAG_NONE);
    UI_DrawParameter(u8g2, parameter_a, "LCD CO-\nNSTRAST", UI_PANEL_SLOT_INDEX_A);
    UI_DrawParameter(u8g2, parameter_b, "LCD CO-\nNSTRAST", UI_PANEL_SLOT_INDEX_B);

    return UI_DRAWING_STATUS_OK;
}
