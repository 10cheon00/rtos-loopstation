#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "rtos_enum_value.h"

#define STATE_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue rtos_enum_value_button_id;
  RtosEnumValue rtos_enum_value_button_state;
} ButtonPayload;

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue rtos_enum_valud_encoder_id;
  int32_t delta;
} EncoderRotationPayload;

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue rtos_enum_value_knob_id;
  uint16_t adc_value;
} AdcConversionPayload;

typedef struct {
  RtosEnumValue rtos_enum_value_state_event_type;
  union {
    ButtonPayload button;
    EncoderRotationPayload encoder_rotation;
    AdcConversionPayload adc_conversion;
  } payload;
} RtosMessage_StateEvent;

#endif
