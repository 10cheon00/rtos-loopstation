#include "ui_renderer_home_panel.h"

#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawHomePanel(u8g2_t *u8g2, Parameter *parameters)
{
    UI_DrawPanelLayout(u8g2, "HOME", UI_ARROW_FLAG_LEFT | UI_ARROW_FLAG_RIGHT );
    u8g2_SendBuffer(u8g2);
    return UI_DRAWING_STATUS_OK;
}
