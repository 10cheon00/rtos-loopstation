#ifndef MCP23017_GPIO_MAP_H
#define MCP23017_GPIO_MAP_H

#include <stddef.h>
#include <stdint.h>

#include "button_id.h"
#include "mcp23017_gpio_id.hpp"
#include "mcp23017_types.hpp"

#define MCP23017_ADDRESS_0B100 (0x20 | 0b100)
#define MCP23017_ADDRESS_0B101 (0x20 | 0b101)

#define MCP23017_GPIO_PORT_A 0x0
#define MCP23017_GPIO_PORT_B 0x1

enum class Mcp23017GpioType {
  INPUT = 0,
  OUTPUT,
};

enum class TrackLedColor {
  NONE = 0,
  RED,
  GREEN,
  BLUE,
  COUNT,
};

struct ParameterPinMapEntry {
  Mcp23017GpioId gpio_id;
  Mcp23017GpioType gpio_type;
  Mcp23017Address address;
  Mcp23017Port port;
  uint8_t pin_index;
};

ParameterPinMapEntry* Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id);
ParameterPinMapEntry* Mcp23017GpioMap_GetTrackLedEntry(uint8_t track_index,
                                                       TrackLedColor color);
Mcp23017GpioId Mcp23017GpioMap_GetMcp23017GpioId(Mcp23017Address address,
                                                 Mcp23017Port port,
                                                 uint8_t pin_index);
Mcp23017PinMask Mcp23017GpioMap_GetInputPinMask(Mcp23017Address address,
                                                Mcp23017Port port);
Mcp23017Port Mcp23017GpioMap_GetPortFromAddressAndGpioId(
    Mcp23017Address address, Mcp23017GpioId gpio_id);
ButtonId Mcp23017GpioMap_Get(Mcp23017GpioId gpio_id);
#endif
