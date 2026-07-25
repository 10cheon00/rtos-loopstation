#include "input_config.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

const Mcp23017DeviceConfig input_mcp23017_devices[] = {
    {.gpio_pin = GPIO_PIN_0, .address = 0x24, .pin_status = 0xFFDF},
    {.gpio_pin = GPIO_PIN_1, .address = 0x25, .pin_status = 0xFFFF}};
const uint8_t input_mcp23017_device_count = ARRAY_COUNT(input_mcp23017_devices);

const InputButtonMapping input_button_mappings[] = {
    {
        .slave_address = 0x24,
        .ids = {
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_ENCODER_PUSH,
            CONTROL_BUTTON_ID_IFX_A_TOGGLE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_TRACK_1_PLAY_RECORD,
            CONTROL_BUTTON_ID_TRACK_1_STOP
        }
    },
    {
        .slave_address = 0x25, 
        .ids = {
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_LEFT,
            CONTROL_BUTTON_ID_RIGHT,
            CONTROL_BUTTON_ID_ENTER,
            CONTROL_BUTTON_ID_EXIT,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_TFX_A_TOGGLE,
            CONTROL_BUTTON_ID_NONE,
            CONTROL_BUTTON_ID_NONE
        }
    }
};
const uint8_t input_button_mapping_count = ARRAY_COUNT(input_button_mappings);
