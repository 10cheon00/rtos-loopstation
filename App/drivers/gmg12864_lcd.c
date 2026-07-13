#include "gmg12864_lcd.h"

static int Gmg12864LcdStatus_IsValidInitParams(const Gmg12864Lcd_InitParams *params)
{
    return (params != 0) && (params->hspi != NULL);
}

Gmg12864LcdStatus Gmg12864Lcd_Init(Gmg12864LcdHandle_t* handle, Gmg12864Lcd_InitParams *params)
{
    if (!Gmg12864LcdStatus_IsValidInitParams(params)) {
        return GMG12864_LCD_STATUS_ERROR;
    }
    handle->hspi = params->hspi;
    
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

Gmg12864LcdStatus Gmg12864Lcd_DrawText(uint8_t x, uint8_t y, const char *text)
{
    return GMG12864_LCD_STATUS_OK;
}
