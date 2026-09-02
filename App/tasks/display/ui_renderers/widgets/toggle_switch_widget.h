#ifndef TOGGLE_SWITCH_WIDGET_H
#define TOGGLE_SWITCH_WIDGET_H

#include "parameter.h"
#include "u8g2.h"

#define TOGGLE_SWITCH_WIDGET_WIDTH 6
#define TOGGLE_SWITCH_WIDGET_HEIGHT 16

void UiWidget_DrawToggleSwitchWidget(u8g2_t* u8g2, uint8_t x, uint8_t y,
                                     Parameter* parameter);

#endif