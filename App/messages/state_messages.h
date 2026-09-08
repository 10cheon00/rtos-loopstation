#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "enum_raw.h"

#define STATE_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue button_id_raw;
  RtosEnumValue button_state_raw;
} ButtonPayload;

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue encoder_id_raw;
  int32_t delta;
} EncoderRotationPayload;

typedef struct {
  uint32_t timestamp_ticks;
  RtosEnumValue knob_id_raw;
  uint16_t adc_value;
} AdcConversionPayload;

typedef struct {
  RtosEnumValue type_raw;
  union {
    ButtonPayload button;
    EncoderRotationPayload encoder_rotation;
    AdcConversionPayload adc_conversion;
  } payload;
} RtosMessage_StateEvent;

#endif
