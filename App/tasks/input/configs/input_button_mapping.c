#include "input_button_mapping.h"

#include "utils.h"

const InputButtonMapping input_button_mappings[] = {
    {
        .slave_address = 0x24,
        .ids = {
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_ENCODER_A_PUSH,
            BUTTON_ID_IFX_A_TOGGLE,
            BUTTON_ID_NONE,
            BUTTON_ID_TRACK_1_PLAY_RECORD,
            BUTTON_ID_TRACK_1_STOP
        }
    },
    {
        .slave_address = 0x25, 
        .ids = {
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE,
            BUTTON_ID_LEFT,
            BUTTON_ID_RIGHT,
            BUTTON_ID_ENTER,
            BUTTON_ID_EXIT,
            BUTTON_ID_NONE,
            BUTTON_ID_TFX_A_TOGGLE,
            BUTTON_ID_NONE,
            BUTTON_ID_NONE
        }
    }
};
const uint8_t input_button_mapping_count = ARRAY_COUNT(input_button_mappings);
