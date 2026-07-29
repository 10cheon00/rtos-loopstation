#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "control_button_id.h"

typedef enum {
  STATE_EVENT_CONTROL_BUTTON = 0,
  STATE_EVENT_ENCODER_ROTATION,
} StateEventType;

typedef struct {
  uint32_t timestamp_ms;
  ControlButtonId id;
  ControlButtonState state;
} ControlButtonPayload;

typedef struct {
    uint32_t timestamp_ms;
    uint8_t encoder_id;
    int32_t delta;
} EncoderRotationPayload;

typedef struct {
  StateEventType type;
  union {
    ControlButtonPayload control_button;
    EncoderRotationPayload encoder_rotation;
  } payload;
} StateEvent;

#endif
