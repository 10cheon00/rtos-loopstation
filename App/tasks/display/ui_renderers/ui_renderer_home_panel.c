#include "ui_renderer_home_panel.h"

#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawHomePanel(u8g2_t *u8g2, Parameter *parameter_a,
                                 Parameter *parameter_b, Parameter *parameter_c,
                                 Parameter *parameter_d)
{
    UI_DrawPanelLayout(u8g2, "HOME", UI_ARROW_FLAG_LEFT | UI_ARROW_FLAG_RIGHT );
    return UI_DRAWING_STATUS_OK;
}
