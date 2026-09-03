#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "encoder_id.h"

#define STATE_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef enum {
  STATE_EVENT_BUTTON = 0,
  STATE_EVENT_ENCODER_ROTATION,
  STATE_EVENT_ADC_CONVERSION,
} StateEventType;

typedef struct {
  uint32_t timestamp_ticks;
  EnumRaw button_id_raw;
  EnumRaw button_state_raw;
} ButtonPayload;

typedef struct {
  uint32_t timestamp_ticks;
  EncoderId encoder_id;
  int32_t delta;
} EncoderRotationPayload;

typedef struct {
  uint32_t timestamp_ticks;
  EnumRaw knob_id_raw;
  uint16_t adc_value;
} AdcConversionPayload;

typedef struct {
  StateEventType type;
  union {
    ButtonPayload button;
    EncoderRotationPayload encoder_rotation;
    AdcConversionPayload adc_conversion;
  } payload;
} StateEvent;

#endif
