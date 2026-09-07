#ifndef STATE_EVENT_TYPE_HPP
#define STATE_EVENT_TYPE_HPP

#include "enum_id.hpp"

enum class StateEventType : EnumId {
  BUTTON = 0,
  ENCODER_ROTATION,
  ADC_CONVERSION,
};

#endif
