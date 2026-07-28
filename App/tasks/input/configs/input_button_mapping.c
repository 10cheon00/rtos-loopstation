#include "input_button_mapping.h"

#include "utils.h"

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
