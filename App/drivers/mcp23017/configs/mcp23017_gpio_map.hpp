#ifndef MCP23017_GPIO_MAP_H
#define MCP23017_GPIO_MAP_H

#include <cstddef>
#include <cstdint>

#include "button_id.hpp"
#include "enum_map.hpp"
#include "mcp23017_gpio_id.hpp"
#include "mcp23017_types.hpp"

namespace Mcp23017 {

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
  Address address;
  Port port;
  PinIndex pin_index;
};

using PinConfigMap = EnumMap<Mcp23017::GpioId, PinConfig>;

const PinConfigMap& GetPinConfigMap();
const PinConfig& GetTrackLedEntry(std::uint8_t track_index,
                                  TrackLedColor color);
GpioId GetTrackLedGpioId(std::uint8_t track_index, TrackLedColor color);
GpioId FindGpioIdFromPinConfig(Address address, Port port, PinIndex pin_index);
PinMask GetInputPinMaskFromAddressAndPort(Address address, Port port);

}  // namespace Mcp23017

#endif
