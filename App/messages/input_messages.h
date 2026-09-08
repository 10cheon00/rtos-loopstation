#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "enum_raw.h"
#include "encoder_id_raw.h"

#define INPUT_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  TickType_t timestamp_ticks;
  uint16_t gpio_pin;
} RtosPayload_Mcp23017Event;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  RtosEnumValue direction_raw;
  EncoderIdRaw encoder_id_raw;
} RtosPayload_EncoderRotation;

typedef struct {
  TickType_t timestamp_ticks;  // for debugging only
  uint16_t adc_value;
  RtosEnumValue knob_id_raw;
} RtosPayload_AdcConversion;

typedef struct {
  RtosEnumValue type_raw;
  union {
    RtosPayload_Mcp23017Event mcp23017_int_event;
    RtosPayload_EncoderRotation encoder_rotation_event;
    RtosPayload_AdcConversion adc_conversion_event;
  } payload;
} RtosMessage_InputEvent;

#endif
