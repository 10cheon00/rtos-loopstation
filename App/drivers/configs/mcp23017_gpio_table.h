#ifndef MCP23017_GPIO_TABLE_H
#define MCP23017_GPIO_TABLE_H

#include "mcp23017.h"

typedef struct {
    Mcp23017GpioId gpio_id;
    Mcp23017Address address;
    Mcp23017GpioPort port;
    uint8_t pin_register_mask;
} ParameterPinMapEntry;

ParameterPinMapEntry *Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id);

#endif
