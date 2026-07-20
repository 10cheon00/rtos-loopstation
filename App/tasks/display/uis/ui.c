#include "ui.h"

void UI_DrawArrowLeft4x5(u8g2_t *u8g2, uint8_t x, uint8_t y)
{
    u8g2_DrawVLine(u8g2, x, y - 3, 1);
    u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 3, y - 5, 5);
}

void UI_DrawArrowRight4x5(u8g2_t *u8g2, uint8_t x, uint8_t y)
{
    u8g2_DrawVLine(u8g2, x, y - 5, 5);
    u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 3, y - 3, 1);
}
