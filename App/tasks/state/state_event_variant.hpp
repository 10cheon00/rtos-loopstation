#ifndef STATE_EVENT_VARIANT_HPP
#define STATE_EVENT_VARIANT_HPP

#include <cstdint>
#include <variant>

#include "FreeRTOS.h"
#include "button_id.hpp"
#include "button_state.hpp"
#include "encoder_id.hpp"
#include "knob_id.hpp"

struct ButtonEvent {
  TickType_t timestamp_ticks;
  ButtonId id;
  ButtonState state;
};

struct EncoderRotationEvent {
  TickType_t timestamp_ticks;
  EncoderId id;
  int32_t delta;
};

struct AdcConversionEvent {
  KnobId id;
  uint16_t adc_value;
};

using StateEventVariant =
    std::variant<ButtonEvent, EncoderRotationEvent, AdcConversionEvent>;

#endif
