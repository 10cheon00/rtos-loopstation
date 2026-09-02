#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "button_id.h"
#include "encoder_id.h"

#define STATE_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef enum {
  STATE_EVENT_BUTTON = 0,
  STATE_EVENT_ENCODER_ROTATION,
  STATE_EVENT_ADC_CONVERSION,
} StateEventType;

typedef struct {
  uint32_t timestamp_ticks;
  ButtonId id;
  ButtonState state;
} ButtonPayload;

typedef struct {
  uint32_t timestamp_ticks;
  EncoderId encoder_id;
  int32_t delta;
} EncoderRotationPayload;

/**
 * enum class로 구현된 KnobId C로 잠시 변환하는 DTO 역할을 맡기 위해 정의된 타입
 */
typedef uint8_t KnobIdRaw;

typedef struct {
  uint32_t timestamp_ticks;
  KnobIdRaw knob_id;
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
