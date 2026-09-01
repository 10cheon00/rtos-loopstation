#ifndef MCP23017_TYPES_HPP
#define MCP23017_TYPES_HPP

#include <cstddef>

enum class Mcp23017Status : uint8_t {
    OK = 0,
    NOT_INITED,
    BUSY,
    ERROR,
};

using Mcp23017Address = uint8_t;
using Mcp23017PinMask = uint8_t;
using Mcp23017PinStates = uint8_t;
using Mcp23017Port = uint8_t;
using Mcp23017InterruptPin = uint16_t;

#endif
