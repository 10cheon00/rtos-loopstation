#ifndef MCP23017_GPIO_TO_BUTTON_MAP_HPP
#define MCP23017_GPIO_TO_BUTTON_MAP_HPP

#include "button_id.hpp"
#include "mcp23017_gpio_id.hpp"

namespace Mcp23017GpioToButtonMap {

ButtonId Get(Mcp23017::GpioId gpio_id);

}

#endif
