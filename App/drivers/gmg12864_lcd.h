#ifndef GMG12864_LCD_H
#define GMG12864_LCD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GMG12864_LCD_STATUS_OK = 0,
    GMG12864_LCD_STATUS_ERROR
} Gmg12864LcdStatus;

Gmg12864LcdStatus Gmg12864Lcd_Init(void);
Gmg12864LcdStatus Gmg12864Lcd_WriteCommand(uint8_t command);
Gmg12864LcdStatus Gmg12864Lcd_WriteData(const uint8_t *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
