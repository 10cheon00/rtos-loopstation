#include "mcp23017_gpio_table.h"

#include "utils.h"

static ParameterPinMapEntry parameter_pin_map[] = {
    {.gpio_id = MCP23017_GPIO_ID_LED_IFX_A,
     .address = MCP23017_ADDRESS_0B100,
     .port = MCP23017_GPIO_PORT_B,
     .pin_register_mask = 0x04},
    {.gpio_id = MCP23017_GPIO_ID_LED_TFX_A,
     .address = MCP23017_ADDRESS_0B101,
     .port = MCP23017_GPIO_PORT_B,
     .pin_register_mask = 0x10},
};

static const size_t parameter_pin_map_count = ARRAY_COUNT(parameter_pin_map);

ParameterPinMapEntry *Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id)
{
    for (size_t i = 0; i < parameter_pin_map_count; i++) {
        if (parameter_pin_map[i].gpio_id == gpio_id) {
            return &parameter_pin_map[i];
        }
    }
    return NULL;
}
