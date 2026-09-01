#ifndef MCP23017_GPIO_MAP_H
#define MCP23017_GPIO_MAP_H

#include <stddef.h>
#include <stdint.h>

#include "button_id.h"
#include "mcp23017_types.hpp"
#include "mcp23017_gpio_id.hpp"

#define MCP23017_ADDRESS_0B100 (0x20 | 0b100)
#define MCP23017_ADDRESS_0B101 (0x20 | 0b101)

#define MCP23017_GPIO_PORT_A 0x0
#define MCP23017_GPIO_PORT_B 0x1

typedef enum  {
    MCP23017_GPIO_TYPE_INPUT,
    MCP23017_GPIO_TYPE_OUTPUT,
} Mcp23017GpioType;

typedef enum {
    TRACK_LED_COLOR_NONE = 0,
    TRACK_LED_COLOR_RED,
    TRACK_LED_COLOR_GREEN,
    TRACK_LED_COLOR_BLUE,
    TRACK_LED_COLOR_COUNT,
} TrackLedColor;

typedef struct {
    Mcp23017GpioId gpio_id;
    Mcp23017GpioType gpio_type;
    Mcp23017Address address;
    Mcp23017Port port;
    uint8_t pin_index;
} ParameterPinMapEntry;

ParameterPinMapEntry *Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id);
ParameterPinMapEntry *Mcp23017GpioMap_GetTrackLedEntry(uint8_t track_index, TrackLedColor color);
Mcp23017GpioId Mcp23017GpioMap_GetMcp23017GpioId(Mcp23017Address address, Mcp23017Port port, uint8_t pin_index);
Mcp23017PinMask Mcp23017GpioMap_GetInputPinMask(Mcp23017Address address, Mcp23017Port port);
Mcp23017Port Mcp23017GpioMap_GetPortFromAddressAndGpioId(Mcp23017Address address, Mcp23017GpioId gpio_id);
ButtonId Mcp23017GpioMap_Get(Mcp23017GpioId gpio_id);
#endif
