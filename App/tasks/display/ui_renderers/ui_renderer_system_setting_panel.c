#include "ui_renderer_setting_panel.h"

#include "knob_widget.h"
#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawSystemSettingPanel(u8g2_t *u8g2, Parameter *parameters)
{
    // 여기서는 인자로 전달된 시스템 밝기를 수정함
    uint8_t value = parameters[0].current;
    u8g2_SetContrast(u8g2, value);

    UI_DrawPanelLayout(u8g2, "SYSTEM SETTING", UI_ARROW_FLAG_NONE);
    UI_DrawParameter(u8g2, &parameters[0], "LCD CO-\nNSTRAST", UI_PANEL_SLOT_INDEX_A);
    UI_DrawParameter(u8g2, &parameters[1], "LCD CO-\nNSTRAST", UI_PANEL_SLOT_INDEX_B);
    
    return UI_DRAWING_STATUS_OK;
}
