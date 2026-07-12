#include "gmg12864_lcd.h"

Gmg12864LcdStatus Gmg12864Lcd_Init(void)
{
    return GMG12864_LCD_STATUS_OK;
}

Gmg12864LcdStatus Gmg12864Lcd_WriteCommand(uint8_t command)
{
    (void)command;
    return GMG12864_LCD_STATUS_OK;
}

Gmg12864LcdStatus Gmg12864Lcd_WriteData(const uint8_t *data, uint16_t length)
{
    if ((data == 0) && (length > 0U)) {
        return GMG12864_LCD_STATUS_ERROR;
    }

    return GMG12864_LCD_STATUS_OK;
}
