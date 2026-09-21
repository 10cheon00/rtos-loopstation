#ifndef TOGGLE_SWITCH_WIDGET_HPP
#define TOGGLE_SWITCH_WIDGET_HPP

#include "parameter.hpp"

namespace UiWidget {

#define TOGGLE_SWITCH_WIDGET_WIDTH 6
#define TOGGLE_SWITCH_WIDGET_HEIGHT 16

void DrawToggleSwitchWidget(uint8_t x, uint8_t y, Parameter& parameter);

}  // namespace UiWidget

#endif
