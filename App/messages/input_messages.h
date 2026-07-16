#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>
#include "FreeRTOS.h"

typedef struct {
    TickType_t timestamp_tick;
    uint16_t gpio_pin;
} Mcp23017IntEvent;

#endif
