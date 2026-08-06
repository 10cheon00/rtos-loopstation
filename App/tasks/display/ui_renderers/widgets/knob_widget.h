#ifndef KNOB_WIDGET_H
#define KNOB_WIDGET_H

#include "u8g2.h"
#include "parameter.h"

void UiWidget_DrawKnobWidget(u8g2_t* u8g2, uint8_t x, uint8_t y, Parameter* current);

#endif