#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>

typedef struct {
    uint32_t timestamp_tick;
    uint8_t i2c_slave_address;
} Mcp23017IntEvent;

#endif
