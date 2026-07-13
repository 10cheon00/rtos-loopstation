#ifndef GMG12864_LCD_H
#define GMG12864_LCD_H

#include <stdint.h>

#include "stm32h7xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GMG12864_LCD_STATUS_OK = 0,
    GMG12864_LCD_STATUS_ERROR
} Gmg12864LcdStatus;

typedef struct {
    SPI_HandleTypeDef *hspi;
} Gmg12864Lcd_InitParams;

typedef struct {
    SPI_HandleTypeDef *hspi;
} Gmg12864LcdHandle_t;

Gmg12864LcdStatus Gmg12864Lcd_Init(Gmg12864LcdHandle_t *handle,
                                   Gmg12864Lcd_InitParams *init_params);
Gmg12864LcdStatus Gmg12864Lcd_WriteCommand(uint8_t command);
Gmg12864LcdStatus Gmg12864Lcd_WriteData(const uint8_t *data, uint16_t length);
Gmg12864LcdStatus Gmg12864Lcd_DrawText(uint8_t x, uint8_t y, const char *text);

#ifdef __cplusplus
}
#endif

#endif
