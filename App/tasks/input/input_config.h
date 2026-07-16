#ifndef INPUT_CONFIG_H
#define INPUT_CONFIG_H

#include <stdint.h>

#include "control_button.h"
#include "mcp23017.h"

typedef struct {
    uint8_t slave_address;
    ControlButtonId ids[16]; // first 8 is port A, second is port B
} InputButtonMapping;

extern const Mcp23017DeviceConfig input_mcp23017_devices[];
extern const uint8_t input_mcp23017_device_count;
extern const InputButtonMapping input_button_mappings[];
extern const uint8_t input_button_mapping_count;

#endif
