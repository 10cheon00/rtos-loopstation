#include "ui.h"

UI_DRAWING_STATUS Ui_DrawHomePanel(u8g2_t *u8g2)
{
    u8g2_ClearBuffer(u8g2);
    u8g2_DrawStr(u8g2, 6, 6, "HOME PANEL");
    u8g2_SendBuffer(u8g2);

    return UI_DRAWING_STATUS_OK;
}