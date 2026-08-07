#ifndef GMG12864_LCD_H
#define GMG12864_LCD_H

#include <stdint.h>

#include "stm32h7xx.h"
#include "u8g2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

typedef enum {
    GMG12864_LCD_STATUS_OK = 0,
    GMG12864_LCD_STATUS_ERROR
} Gmg12864LcdStatus;

typedef struct {
    SPI_HandleTypeDef *hspi;
    uint16_t CS_Pin;
    uint16_t RST_Pin;
    uint16_t DC_Pin;
    GPIO_TypeDef* CS_Port;
    GPIO_TypeDef* RST_Port;
    GPIO_TypeDef* DC_Port;
} Gmg12864Lcd_InitParams;

Gmg12864LcdStatus Gmg12864Lcd_Init(u8g2_t *u8g2, Gmg12864Lcd_InitParams *init_params);

#ifdef __cplusplus
}
#endif

#endif
