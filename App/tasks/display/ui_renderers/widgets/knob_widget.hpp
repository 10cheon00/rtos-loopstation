#ifndef KNOB_WIDGET_HPP
#define KNOB_WIDGET_HPP

#include "parameter.h"
#include "u8g2.h"

namespace UiWidget {

#define KNOB_WIDGET_WIDTH 16
#define KNOB_WIDGET_HEIGHT 16

void DrawKnobWidget(u8g2_t* u8g2, uint8_t x, uint8_t y, Parameter* current);

}  // namespace UiWidget

#endif
