#ifndef MCP23017_GPIO_MAP_H
#define MCP23017_GPIO_MAP_H

#include <cstddef>
#include <cstdint>

#include "button_id.h"
#include "enum_map.hpp"
#include "mcp23017_gpio_id.hpp"
#include "mcp23017_types.hpp"

#define MCP23017_ADDRESS_0B100 (0x20 | 0b100)
#define MCP23017_ADDRESS_0B101 (0x20 | 0b101)

#define MCP23017_GPIO_PORT_A 0x0
#define MCP23017_GPIO_PORT_B 0x1

namespace Mcp23017GpioMap {

enum class Address : Mcp23017Address {
  b100 = (0x20 | 0b100),
  b101 = (0x20 | 0b101),
};

enum class Port : Mcp23017Port {
  A = 0x0,
  B = 0x1,
};

enum class GpioType {
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

struct PinConfig {
  GpioType gpio_type;
  Mcp23017Address address;
  Mcp23017Port port;
  std::uint8_t pin_index;
};

using PinConfigMap = EnumMap<Mcp23017GpioId, PinConfig>;

const PinConfigMap& GetEnumMap();
const PinConfig& GetTrackLedEntry(std::uint8_t track_index,
                                  TrackLedColor color);
Mcp23017GpioId GetTrackLedGpioId(std::uint8_t track_index, TrackLedColor color);
Mcp23017GpioId FindGpioIdFromPinConfig(Mcp23017Address address, Mcp23017Port port, std::uint8_t pin_index);
Mcp23017PinMask GetInputPinMaskFromAddressAndPort(Mcp23017Address address, Mcp23017Port port);
}  // namespace Mcp23017GpioMap

#endif
