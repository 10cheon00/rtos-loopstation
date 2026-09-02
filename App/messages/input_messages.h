#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "encoder_id.h"

#define INPUT_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  TickType_t timestamp_ticks;
  uint16_t gpio_pin;
} Mcp23017IntEvent;

typedef enum {
  INPUT_EVENT_MCP23017 = 0,
  INPUT_EVENT_ENCODER_ROTATION,
  INPUT_EVENT_ADC_CONVERSION,
} InputEventType;

typedef enum {
  ENCODER_ROTATE_CLOCKWISE = 0,
  ENCODER_ROTATE_COUNTER_CLOCKWISE,
} EncoderRotationDirection;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  EncoderRotationDirection direction;
  EncoderId encoder_id;
} EncoderRotationEvent;

/**
 * enum class로 구현된 KnobId C로 잠시 변환하는 DTO 역할을 맡기 위해 정의된 타입
 */
typedef uint8_t KnobIdRaw;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  uint16_t adc_value;
  KnobIdRaw knob_id;
} AdcConversionEvent;

typedef struct {
  InputEventType type;
  union {
    Mcp23017IntEvent mcp23017_int_event;
    EncoderRotationEvent encoder_rotation_event;
    AdcConversionEvent adc_conversion_event;
  } payload;
} InputEvent;

#endif
