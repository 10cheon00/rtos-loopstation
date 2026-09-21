#ifndef KNOB_WIDGET_HPP
#define KNOB_WIDGET_HPP

#include "parameter.hpp"

namespace UiWidget {

#define KNOB_WIDGET_WIDTH 16
#define KNOB_WIDGET_HEIGHT 16

void DrawKnobWidget(uint8_t x, uint8_t y, Parameter& current);

}  // namespace UiWidget

#endif
