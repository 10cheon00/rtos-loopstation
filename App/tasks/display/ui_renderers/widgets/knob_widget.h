#ifndef KNOB_WIDGET_H
#define KNOB_WIDGET_H

#include "u8g2.h"
#include "parameter.h"

#define KNOB_WIDGET_WIDTH 16
#define KNOB_WIDGET_HEIGHT 16

void UiWidget_DrawKnobWidget(u8g2_t* u8g2, uint8_t x, uint8_t y, Parameter* current);

#endif