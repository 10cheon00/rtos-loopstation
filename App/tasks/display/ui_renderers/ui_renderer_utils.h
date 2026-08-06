#ifndef UI_RENDERER_UTILS_H
#define UI_RENDERER_UTILS_H

#include "ui_renderer.h"

void UI_DrawArrowLeft4x5(u8g2_t *u8g2, uint8_t x, uint8_t y);
void UI_DrawArrowRight4x5(u8g2_t *u8g2, uint8_t x, uint8_t y);
void UI_ConvertNumberToString(int32_t number, char* string, uint8_t string_length);

#endif
