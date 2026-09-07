#ifndef INPUT_EVENT_TYPE_HPP
#define INPUT_EVENT_TYPE_HPP

#include "enum_id.hpp"

enum class InputEventType : EnumId {
  MCP23017 = 0,
  ENCODER_ROTATION,
  ADC_CONVERSION,
};

#endif
