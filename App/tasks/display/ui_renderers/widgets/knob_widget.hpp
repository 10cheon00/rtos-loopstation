#ifndef KNOB_WIDGET_HPP
#define KNOB_WIDGET_HPP

#include <cstdint>

#include "parameter.hpp"

namespace UiWidget {

#define KNOB_WIDGET_WIDTH 16
#define KNOB_WIDGET_HEIGHT 16

void DrawKnobWidget(std::uint8_t x, std::uint8_t y, std::int16_t min,
                    std::int16_t max, std::int16_t current);

}  // namespace UiWidget

#endif
