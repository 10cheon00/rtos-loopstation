#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "rtos_enum_value.h"

#define INPUT_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  TickType_t timestamp_ticks;
  uint16_t gpio_pin;
} RtosPayload_Mcp23017Event;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  RtosEnumValue rtos_enum_value_encoder_rotation_direction;
  RtosEnumValue rtos_enum_value_encoder_id;
} RtosPayload_EncoderRotation;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  uint16_t adc_value;
  RtosEnumValue rtos_enum_value_knob_id;
} RtosPayload_AdcConversion;

typedef struct {
  RtosEnumValue rtos_enum_value_input_event_type;
  union {
    RtosPayload_Mcp23017Event mcp23017_int_event;
    RtosPayload_EncoderRotation encoder_rotation_event;
    RtosPayload_AdcConversion adc_conversion_event;
  } payload;
} RtosMessage_InputEvent;

#endif
