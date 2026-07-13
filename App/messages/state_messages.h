#ifndef STATE_MESSAGES_H
#define STATE_MESSAGES_H

#include <stdint.h>

typedef enum {
  STATE_EVENT_CONTROL_BUTTON = 0
} StateEventType;

typedef enum {
  CONTROL_BUTTON_STATE_RELEASED = 0,
  CONTROL_BUTTON_STATE_PRESSED
} ControlButtonState;

typedef struct {
  uint16_t button_id;
  ControlButtonState state;
  uint32_t timestamp_ms;
} ControlButtonPayload;

typedef struct {
  StateEventType type;
  union {
    ControlButtonPayload control_button;
  } payload;
} StateEvent;

#endif
