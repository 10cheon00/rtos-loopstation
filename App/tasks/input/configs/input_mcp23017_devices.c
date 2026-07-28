#include "input_mcp23017_devices.h"

#include "utils.h"

const Mcp23017DeviceConfig input_mcp23017_devices[] = {
    {.gpio_pin = GPIO_PIN_0, .address = 0x24, .pin_status = 0xFFDF},
    {.gpio_pin = GPIO_PIN_1, .address = 0x25, .pin_status = 0xFFFF}};

const uint8_t input_mcp23017_device_count = ARRAY_COUNT(input_mcp23017_devices);
