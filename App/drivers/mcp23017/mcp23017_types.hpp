#ifndef MCP23017_TYPES_HPP
#define MCP23017_TYPES_HPP

#include <cstddef>

namespace Mcp23017 {

enum class Status : uint8_t {
  OK = 0,
  NOT_INITED,
  BUSY,
  ERROR,
};

enum class Address : std::uint8_t {
  NONE = 0,
  b100 = (0x20 | 0b100),
  b101 = (0x20 | 0b101),
};
using PinMask = std::uint8_t;
using PinStates = std::uint8_t;
enum class Port : std::uint8_t {
  A = 0x0,
  B = 0x1,
};
using InterruptPin = std::uint16_t;
using PinIndex = std::uint8_t;

}  // namespace Mcp23017

#endif
