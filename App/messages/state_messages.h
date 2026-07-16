#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

#include "control_button.h"

typedef enum {
  STATE_EVENT_CONTROL_BUTTON = 0
} StateEventType;

typedef struct {
  uint32_t timestamp_ms;
  ControlButtonId id;
  ControlButtonState state;
} ControlButtonPayload;

typedef struct {
  StateEventType type;
  union {
    ControlButtonPayload control_button;
  } payload;
} StateEvent;

#endif
