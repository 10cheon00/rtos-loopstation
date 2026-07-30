#ifndef UI_H
#define UI_H

#include "gmg12864_lcd.h"

typedef enum {
    UI_DRAWING_STATUS_OK = 0,
    UI_DRAWING_STATUS_ERROR,
} UI_DRAWING_STATUS;

extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

void UI_DrawArrowLeft4x5(u8g2_t *u8g2, uint8_t x, uint8_t y);
void UI_DrawArrowRight4x5(u8g2_t *u8g2, uint8_t x, uint8_t y);

#endif
