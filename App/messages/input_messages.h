#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>
#include "FreeRTOS.h"

typedef struct {
    TickType_t timestamp_tick;
    uint16_t gpio_pin;
} Mcp23017IntEvent;

typedef enum {
    INPUT_EVENT_MCP23017 = 0,
    INPUT_EVENT_ENCODER_ROTATION,
} InputEventType;

typedef enum {
    ENCODER_ROTATE_CLOCKWISE = 0,
    ENCODER_ROTATE_COUNTER_CLOCKWISE = 0,
} EncoderRotationDirection;

typedef struct {
    TickType_t timestamp_tick; // for debugging only
    uint32_t encoder_counter;
    uint8_t encoder_id;
} EncoderRotationEvent;

typedef struct {
    InputEventType type;
    union {
        Mcp23017IntEvent mcp23017_int_event;
        EncoderRotationEvent encoder_rotation_event;
    } payload;
} InputEvent;

#endif
